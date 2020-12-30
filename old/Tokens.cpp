//
// Created by dev on 2016-04-03.
//
// Based off of the following language
//S-> P (B P)*
//P-> V | "(" S ")" | U P | F "(" S ")" | T "(" E*N*E* "," S "," S ")"
//B->"=" | "!=" | ">" | "<" | "+" | "-" | "*" | "/" | "^"
//U-> "-" 
//F->"log" | "ln" | "sin" | "cos" | "tan" | "sinh" | "cosh" | "tanh" | "asin" | "acos" | "atan" | "sqrt"| "ceil" | "floor"| "abs"
//T->"sum" | "mult"
//V->E*N*E* � F | N*.?N*
//E->[a-z|A-Z]
//N->[0-9]
//

#include "Tokens.h"
#include <string>

using namespace std;

std::string ElaroDARFormula::stringToLower(string target)
{
	for (unsigned int i = 0; i<target.length(); i++)
	{
		target[i] = (char)tolower(target[i]);
	}
	return target;
}

ElaroDARFormula::Token::Token(string element, int level) {
    _element=element;
    _level=level;
}

ElaroDARFormula::Token::Token(char element, int level) {
    _element=element;
    _level=level;
}

ElaroDARFormula::Token::Token(const Token &that){
    this->_element=that._element;
    this->_level=that._level;
}

ElaroDARFormula::Token::~Token() {

}

const string ElaroDARFormula::Token::toString() {
    stringstream result;
    result << _element << "," << _level;
    return result.str();
}

const string ElaroDARFormula::Token::getElement() {
    return _element;
}

int ElaroDARFormula::Token::getLevel() {
    return _level;
}

bool ElaroDARFormula::Token::operator==(Token that) {
        return ((this->getElement()==that.getElement())&&(this->getLevel()==that.getLevel()));
}

void ElaroDARFormula::Token::setElement(const string &element) {
    Token::_element = element;
}

void ElaroDARFormula::Token::setLevel(int level) {
    Token::_level = level;
}


void ElaroDARFormula::FormulaTokens::expect(ElaroDARFormula::Token testee, ElaroDARFormula::Token expected){
    if (testee == expected) {
        return;
    } else if (expected.getElement()==")")
    {
        throw ElaroDARFormula::UnevnParenException("Missing closing parentheses");
    } else
        throw ElaroDARFormula::UnxpctdTknException("Unexpected symbol: "+testee.getElement());
}

vector<ElaroDARFormula::Token>::iterator ElaroDARFormula::FormulaTokens::s(vector<Token>::iterator it){
    p(it);
    if(it!=this->end())
        ++it;
    while(it!=this->end()&& (it->getLevel()>0 && it->getLevel()<5))
    {
        p(it);
        if(it!=this->end())
        ++it;    
    }
    return it;
}

void ElaroDARFormula::FormulaTokens::p(vector<Token>::iterator it){
    if(it!=this->end())
        ++it;
    else
        throw ElaroDARFormula::UnxpctdEOLException("Unexpected end of formula");

    if(it->getLevel()==0)
        return;
	else if (it->getElement() == "sum" || it->getElement() == "mult")
	{
		if (it!=this->end())
			++it;
		else
			throw ElaroDARFormula::UnxpctdEOLException("Unexpected end of formula");
		try {
			expect(*it,Token("(", 7));
		}
		catch (runtime_error e)
		{
			throw e;
		}
		if (it!=this->end())
			++it;
		else
			throw ElaroDARFormula::UnxpctdEOLException("Unexpected end of formula");
		if (it->getLevel() != 0)
			throw ElaroDARFormula::UnxpctdTknException("Unexpected symbol: Expected a variable, got " + it->getElement());
		if (it!=this->end())
			++it;
		else
			throw ElaroDARFormula::UnxpctdEOLException("Unexpected end of formula");
		expect(*it,Token(",", 7));
		s(it);
		expect(*it,Token(",", 7));
		s(it);
		expect(*it,Token(")", 7));
		return;
	}
    else if(it->getElement()=="(")
    {
        s(it);
        if(it->getElement()!=")")
            throw ElaroDARFormula::UnevnParenException("Open parentheses not closed");
        return;
    }
    else if(it->getElement()=="-")
    {
        it->setElement("--");
        it->setLevel(5);
        p(it);
    }
    else if(it->getLevel()==6)
    {
        if(it!=this->end())
            ++it;
        else
            throw ElaroDARFormula::UnxpctdEOLException("Unexpected end of formula");
        try {
            expect(*it,Token("(", 7));
		}
		catch (runtime_error e)
		{
			throw e;
		}
        s(it);
        try {
            expect(*it,Token(")", 7));
        }catch(runtime_error e)
        {
            throw e;
        }
    }
}

ElaroDARFormula::FormulaTokens::FormulaTokens() {
}

ElaroDARFormula::FormulaTokens::~FormulaTokens() {
	
}

ElaroDARFormula::FormulaTokens* ElaroDARFormula::FormulaTokens::Tokenize(string expression)
{
    FormulaTokens *result=new FormulaTokens();

    if(expression.length()==0)
        throw ElaroDARFormula::UnxpctdEOLException("Formula is empty");
    for(unsigned int i=0;i<expression.length();i++)
    {
        char current=expression[i];
        if(isdigit(current)||current=='.')
        {
            unsigned int j=1;
            for(j=1;i+j<expression.length() && (isdigit(expression[i+j])|| expression[i+j]=='.');j++)
            {}
            result->push_back(*new Token(expression.substr(i, j), 0));
            i+=j-1;
        }
        else if(isalpha(current)||current=='_')
        {
            unsigned int j;
            for(j=1;i+j<expression.length() && (isalpha(expression[i+j]) || expression[i+j]=='_');j++)
            {}
            string temp=expression.substr(i,j);
            temp=stringToLower(temp);
            if(temp=="sqrt"|| temp=="log"|| temp=="ln"||temp=="sin"||temp=="cos"||temp=="tan"||temp=="sinh"||
                        temp=="cosh"|| temp=="tanh"|| temp=="asin"|| temp=="acos"|| temp=="atan"|| temp=="abs"||
                        temp=="ceil"|| temp=="floor" || temp=="sum" || temp=="mult")
            {
                result->push_back(*new Token(temp, 6));
            }
            else
            {
                unsigned int k=i+j;
                for(j=0;k+j<expression.length() && (isalnum(expression[k+j])||expression[k+j]=='_');j++)
                {}
                temp.append(expression.substr(k,j));
                result->push_back(*(new Token(temp,0)));
                i=k+j-1;
            }
        }
        else if(current=='='|| current=='!' || current=='>' || current=='<')
        {
            if(current=='!')
            {
                result->push_back(*new Token("!=",1));
                i++;
            }
            else
            {
                result->push_back(*new Token(current,1));
            }
        }
        else if(current=='+' || current=='-')
        {
            result->push_back(*new Token(current,2));
        }
        else if(current=='*' || current=='/' || current=='%')
        {
            result->push_back(*new Token(current,3));
        }
        else if(current=='^')
        {
            result->push_back(*new Token(current,4));
        }
        else if(current=='(' || current==')' || current==',')
        {
            result->push_back(*new Token(current,7));
        }
        else if(current==' ')
        {
        }
        else if(current=='\0')
        {
            break;
        }
        else
        {
            throw ElaroDARFormula::UnxpctdCharException("Unexpected Character: "+current);
        }
    }
    
    return result;
}

ElaroDARFormula::FormulaTokens* ElaroDARFormula::FormulaTokens::checkFormula(){
    cout << this->toString() << endl;
    vector<Token>::iterator it=this->begin();
    s(it);
    if(it!=this->end())
        throw ElaroDARFormula::UnxpctdTknException("Unexpected symbol: "+it->getElement());
    return this;
}

ElaroDARFormula::FormulaTokens* ElaroDARFormula::FormulaTokens::checkVariables(vector<string> expectedVars)
{

    for(vector<Token>::iterator it=this->begin();it!=this->end();it++)
    {
        if(it->getLevel()==0)
        {
            if(regex_match(it->getElement(),regex("[0-9]*(.?)[0-9]*")))
                continue;
            bool varChecksOut=false;
            for(vector<string>::iterator i=expectedVars.begin();i!=expectedVars.end();i++)
            {
                if(it->getElement()==*(i) || stringToLower(it->getElement())=="e" ||
                        stringToLower(it->getElement())=="pi" || it->getElement()=="r")
                {
                    varChecksOut = true;
                    break;
                }
            }
            if(varChecksOut)
                continue;
            else throw ElaroDARFormula::UnxpctdVarException(it->getElement());
        }
    }
    return this;
}

ElaroDARFormula::FormulaTokens* ElaroDARFormula::FormulaTokens::makePostfix() {
    FormulaTokens* result=new FormulaTokens();
    stack<Token>* operatorStack = new stack<Token>();


    for(vector<Token>::iterator it=this->begin();it!=this->end();it++)
    {
        if(it->getLevel()==0)
        {
            result->push_back(*it);
        }
        else if(operatorStack->empty())
        {
            operatorStack->push(*it);
        }
        else if(it->getElement()==")")
        {
            while(!operatorStack->empty()&&!(operatorStack->top().getElement()=="("))
            {
                result->push_back(*(new Token(operatorStack->top())));
                operatorStack->pop();
            }
            operatorStack->pop();
        }
        else if(operatorStack->top().getLevel()<it->getLevel())
        {
            operatorStack->push(*it);
        }
        else
        {
            while(!operatorStack->empty()&&operatorStack->top().getLevel()>=it->getLevel()&&
                    operatorStack->top().getLevel()<7)
            {
                result->push_back(*(new Token(operatorStack->top())));
                operatorStack->pop();
            }
            operatorStack->push(*it);
        }
    }

    while(!operatorStack->empty())
    {
        result->push_back(*(new Token(operatorStack->top())));
        operatorStack->pop();
    }

    delete operatorStack;
    return result;
}

std::string ElaroDARFormula::FormulaTokens::toString(){
    
    std::string expression="";
    for(vector<Token>::iterator it=this->begin();it!=this->end();it++)
    {
        expression.append(it->toString()+" ");
    }
    return expression;
}