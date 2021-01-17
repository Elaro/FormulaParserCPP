#ifndef ElaroSolutions_DARFormula_PARSER_CPP
#define ElaroSolutions_DARFormula_PARSER_CPP

#include <wchar.h>
#include <typeinfo>
#include "Parser.h"
#include "Scanner.h"


namespace ElaroSolutions {
namespace DARFormula {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::DARFormula() {
		Node *e=nullptr;
		Formula(e);
		this->root = e;
}

void Parser::Formula(Node *e) {
		BinaryFunctions op = (BinaryFunctions)Undefined;
		Node *e2=nullptr;
		Expression(e);
		while (la->kind == _comp) {
			Get();
			if(t->val==(wchar_t*)"=")
			{op = Equals;}
			else if(t->val==(wchar_t*)"!=")
			{op = Unequals;}
			else if(t->val==(wchar_t*)"<")
			{op = Lesser;}
			else if(t->val==(wchar_t*)">")
			{op = Greater;}
			
			Expression(e2);
			e = BinaryNode::BinaryNodeConstructor(e,e2,op); 
		}
}

void Parser::Expression(Node *e) {
		BinaryFunctions op = (BinaryFunctions)Undefined;
		Node *e2=nullptr;
		Term(e);
		while (la->kind == _plus || la->kind == _minus) {
			if (la->kind == _plus) {
				Get();
				op = Plus; 
			} else {
				Get();
				op = Minus; 
			}
			Term(e2);
			e = BinaryNode::BinaryNodeConstructor(e,e2,op); 
		}
}

void Parser::Term(Node *e) {
		BinaryFunctions op = (BinaryFunctions)Undefined; 
		Node *e2=nullptr;
		Factor(e);
		while (la->kind == _muldivmod) {
			Get();
			if(t->val==(wchar_t*)"*")
			  {op = Times;}
			else if(t->val==(wchar_t*)"/")
			  {op = Divide;}
			else if(t->val==(wchar_t*)"%")
			  {op = Modulo;}
			
			Factor(e2);
			e = BinaryNode::BinaryNodeConstructor(e,e2,op); 
		}
}

void Parser::Factor(Node *e) {
		Node *e2=nullptr;
		PossiblyNegatedOperand(e);
		while (la->kind == _exp) {
			Get();
			PossiblyNegatedOperand(e2);
			e = BinaryNode::BinaryNodeConstructor(e,e2, Exponent); 
		}
}

void Parser::PossiblyNegatedOperand(Node *e) {
		bool isNegated = false; 
		if (la->kind == _minus) {
			Get();
			isNegated = true; 
		}
		Operand(e);
		if(isNegated){e = UnaryNode::UnaryNodeConstructor(e,Negate); }
}

void Parser::Operand(Node *e) {
		if (la->kind == _variable || la->kind == _data || la->kind == _number) {
			Quantity(e);
		} else if (StartOf(1)) {
			UnaryFunctions op=(UnaryFunctions)Undefined;
			Func(op);
			Expect(_leftparen);
			Formula(e);
			Expect(_rightparen);
			e = UnaryNode::UnaryNodeConstructor(e,op);
		} else if (la->kind == _sum || la->kind == _mult) {
			TernaryFunctions op=Undefined;
			std::string countingVariable; 
			Node *lim;
			Func3(op);
			Expect(_leftparen);
			Expect(_variable);
			countingVariable.append((const char *)t->val); 
			Expect(_comma);
			Formula(lim);
			Expect(_comma);
			Formula(e);
			Expect(_rightparen);
			e = TernaryNode::TernaryNodeConstructor(countingVariable,lim,e,op); 
		} else if (la->kind == _leftparen) {
			Get();
			Formula(e);
			Expect(_rightparen);
		} else SynErr(32);
}

void Parser::Quantity(Node *e) {
		if (la->kind == _data) {
			Data((DataNode *)e);
		} else if (la->kind == _variable) {
			Get();
			e = new VariableNode(std::string((const char *)t->val),variables);
		} else if (la->kind == _number) {
			Get();
			e = new ValueNode((double)std::atof((const char *)t->val));
		} else SynErr(33);
}

void Parser::Func(UnaryFunctions &op) {
		switch (la->kind) {
		case _sin: {
			Get();
			break;
		}
		case _cos: {
			Get();
			break;
		}
		case _tan: {
			Get();
			break;
		}
		case _asin: {
			Get();
			break;
		}
		case _acos: {
			Get();
			break;
		}
		case _atan: {
			Get();
			break;
		}
		case _sinh: {
			Get();
			break;
		}
		case _cosh: {
			Get();
			break;
		}
		case _tanh: {
			Get();
			break;
		}
		case _log: {
			Get();
			break;
		}
		case _ln: {
			Get();
			break;
		}
		case _sqrt: {
			Get();
			break;
		}
		case _ceil: {
			Get();
			break;
		}
		case _floor: {
			Get();
			break;
		}
		case _abs: {
			Get();
			break;
		}
		default: SynErr(34); break;
		}
		op = (UnaryFunctions)t->kind; 
}

void Parser::Func3(TernaryFunctions &op) {
		if (la->kind == _sum) {
			Get();
		} else if (la->kind == _mult) {
			Get();
		} else SynErr(35);
		op = (TernaryFunctions)t->kind;
}

void Parser::Data(DataNode *e) {
		std::vector<Node*> *indexes = new std::vector<Node*>();
		std::string fieldName = "";
		Node *e1=nullptr;
		Node *e2=nullptr; 
		Expect(_data);
		Expect(_leftbrack);
		Formula(e1);
		Expect(_rightbrack);
		indexes->push_back(e1); 
		while (la->kind == _leftbrack) {
			Get();
			Formula(e2);
			Expect(_rightbrack);
			indexes->push_back(e2); 
		}
		if (la->kind == _colon) {
			Get();
			Expect(_variable);
			fieldName.append((const char *)t->val); 
		}
		e = new DataNode(*indexes, fieldName);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

Node* Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	DARFormula();
	Expect(0);
	return this->root;
}

Parser::Parser(Scanner *scanner, std::unordered_map<std::string,double> *variables) {
	maxT = 31;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	this->variables = variables;
	this->root = nullptr;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[2][33] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"variable expected"); break;
			case 2: s = coco_string_create(L"data expected"); break;
			case 3: s = coco_string_create(L"sin expected"); break;
			case 4: s = coco_string_create(L"cos expected"); break;
			case 5: s = coco_string_create(L"tan expected"); break;
			case 6: s = coco_string_create(L"asin expected"); break;
			case 7: s = coco_string_create(L"acos expected"); break;
			case 8: s = coco_string_create(L"atan expected"); break;
			case 9: s = coco_string_create(L"sinh expected"); break;
			case 10: s = coco_string_create(L"cosh expected"); break;
			case 11: s = coco_string_create(L"tanh expected"); break;
			case 12: s = coco_string_create(L"log expected"); break;
			case 13: s = coco_string_create(L"ln expected"); break;
			case 14: s = coco_string_create(L"sqrt expected"); break;
			case 15: s = coco_string_create(L"ceil expected"); break;
			case 16: s = coco_string_create(L"floor expected"); break;
			case 17: s = coco_string_create(L"abs expected"); break;
			case 18: s = coco_string_create(L"sum expected"); break;
			case 19: s = coco_string_create(L"mult expected"); break;
			case 20: s = coco_string_create(L"number expected"); break;
			case 21: s = coco_string_create(L"comp expected"); break;
			case 22: s = coco_string_create(L"plus expected"); break;
			case 23: s = coco_string_create(L"minus expected"); break;
			case 24: s = coco_string_create(L"muldivmod expected"); break;
			case 25: s = coco_string_create(L"exp expected"); break;
			case 26: s = coco_string_create(L"comma expected"); break;
			case 27: s = coco_string_create(L"leftparen expected"); break;
			case 28: s = coco_string_create(L"rightparen expected"); break;
			case 29: s = coco_string_create(L"leftbrack expected"); break;
			case 30: s = coco_string_create(L"rightbrack expected"); break;
			case 31: s = coco_string_create(L"??? expected"); break;
			case 32: s = coco_string_create(L"invalid Operand"); break;
			case 33: s = coco_string_create(L"invalid Quantity"); break;
			case 34: s = coco_string_create(L"invalid Func"); break;
			case 35: s = coco_string_create(L"invalid Func3"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- pos %d: %ls\n", col, s);
	summaryFormatter << (L"-- pos %d: %ls\n", col, s);
	summary.append(summaryFormatter.str());
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- pos %d: %ls\n", line, col, s);
	summaryFormatter << (L"-- pos %d: %ls\n", col, s);
	summary.append(summaryFormatter.str());
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- pos %d: %ls\n", col, s);
	summaryFormatter << (L"-- pos %d: %ls\n", col, s);
	summary.append(summaryFormatter.str());
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
	summaryFormatter << (L"%ls\n", s);
	summary.append(summaryFormatter.str());
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	throw std::runtime_error((const char *)s);
}

} // namespace
} // namespace

#endif