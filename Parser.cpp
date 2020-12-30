

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"


namespace ElaroSolutions::DARFormula {


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
		Formula(out e);
}

void Parser::Formula(out Node e) {
		Expression(out e);
		while (la->kind == _comp) {
			Get();
			Expression(out e2);
		}
}

void Parser::Expression(out Node e) {
		Term(out e);
		while (la->kind == _plus || la->kind == _minus) {
			if (la->kind == _plus) {
				Get();
			} else {
				Get();
			}
			Term(out e2);
		}
}

void Parser::Term(out Node e) {
		Factor(out e);
		while (la->kind == _muldivmod) {
			Get();
			Factor(out e2);
		}
}

void Parser::Factor(out Node e) {
		PossiblyNegatedOperand(out e);
		while (la->kind == _exp) {
			Get();
			PossiblyNegatedOperand(out e2);
		}
}

void Parser::PossiblyNegatedOperand(out Node e) {
		if (la->kind == _minus) {
			Get();
		}
		Operand(out e);
}

void Parser::Operand(out Node e) {
		if (la->kind == _variable || la->kind == _data || la->kind == _number) {
			Quantity(out e);
		} else if (StartOf(1)) {
			Func(out op);
			Expect(_leftparen);
			Formula(out e);
			Expect(_rightparen);
		} else if (la->kind == _sum || la->kind == _mult) {
			Func3(out op);
			Expect(_leftparen);
			Expect(_variable);
			Expect(_comma);
			Formula(out lim);
			Expect(_comma);
			Formula(out e);
			Expect(_rightparen);
		} else if (la->kind == _leftparen) {
			Get();
			Formula(out e);
			Expect(_rightparen);
		} else SynErr(32);
}

void Parser::Quantity(out LeafNode e) {
		if (la->kind == _data) {
			Data(out e);
		} else if (la->kind == _variable) {
			Get();
		} else if (la->kind == _number) {
			Get();
		} else SynErr(33);
}

void Parser::Func(out UnaryFunctions op) {
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
}

void Parser::Func3(out TernaryFunctions op) {
		if (la->kind == _sum) {
			Get();
		} else if (la->kind == _mult) {
			Get();
		} else SynErr(35);
}

void Parser::Data(out DataNode e) {
		Expect(_data);
		Expect(_leftbrack);
		Formula(out e1);
		Expect(_rightbrack);
		while (la->kind == _leftbrack) {
			Get();
			Formula(out e2);
			Expect(_rightbrack);
		}
		if (la->kind == _leftbrack) {
			Get();
			Expect(_variable);
			Expect(_rightbrack);
		}
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

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	DARFormula();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 31;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
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
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

} // namespace

