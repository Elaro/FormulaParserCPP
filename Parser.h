

#if !defined(ElaroSolutions__DARFormula_COCO_PARSER_H__)
#define ElaroSolutions__DARFormula_COCO_PARSER_H__


#include "Nodes.h"
#include "Scanner.h"

namespace ElaroSolutions {
namespace DARFormula {


class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_variable=1,
		_data=2,
		_sin=3,
		_cos=4,
		_tan=5,
		_asin=6,
		_acos=7,
		_atan=8,
		_sinh=9,
		_cosh=10,
		_tanh=11,
		_log=12,
		_ln=13,
		_sqrt=14,
		_ceil=15,
		_floor=16,
		_abs=17,
		_sum=18,
		_mult=19,
		_number=20,
		_comp=21,
		_plus=22,
		_minus=23,
		_muldivmod=24,
		_exp=25,
		_comma=26,
		_leftparen=27,
		_rightparen=28,
		_leftbrack=29,
		_rightbrack=30,
		_colon=31
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	Node *root;
	std::unordered_map<std::string,double> *variables;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token



	Parser(Scanner *scanner,std::unordered_map<std::string,double> *variables);
	~Parser();
	void SemErr(const wchar_t* msg);

	void DARFormula();
	void Formula(Node *e);
	void Expression(Node *e);
	void Term(Node *e);
	void Factor(Node *e);
	void PossiblyNegatedOperand(Node *e);
	void Operand(Node *e);
	void Quantity(Node *e);
	void Func(UnaryFunctions &op);
	void Func3(TernaryFunctions &op);
	void Data(DataNode *e);

	Node* Parse();

}; // end Parser

} // namespace
} // namespace


#endif

