//
// Created by fldenhezt on 21-03-04.
//

#include "Parser.h"

namespace ElaroSolutions{namespace DARFormula{
    std::vector<Token> ElaroSolutions::DARFormula::Parser::Tokenize(std::wstring &formula) {
        std::vector<Token> formulaTokens;
        if(formula.empty())
        {
            return formulaTokens;
        }
        for(size_t i=0;i<formula.size();++i)
        {
            wchar_t current=formula.at(i);
            if(isdigit(current)||current=='.')
            {
                //Make a number token
                size_t j;
                for(j=1;i+j<formula.size() && (isdigit(formula.at(i+j))||formula.at(i+j)==(wchar_t )'.');++j)
                {}
                formulaTokens.emplace_back(formula.substr(i,i+j),0);
                i+=j-1;
            }
            else if(isalpha(current)||current=='_')
            {
                //Make a variable or predefined function token
                size_t j;
                std::wstring temp;
                for(j=1;i+j<formula.size() && (isalnum(formula.at(i+j))||formula.at(i+j)==(wchar_t)'_');++j)
                {}
                temp = formula.substr(i,j);
                //Here be the functions
                if(temp==(wchar_t *)"sqrt" ||temp==(wchar_t *)"log" ||temp==(wchar_t *)"ln" ||temp==(wchar_t *)"sin" ||
                temp==(wchar_t *)"cos" ||temp==(wchar_t *)"tan" ||temp==(wchar_t *)"sinh" ||temp==(wchar_t *)"cosh" ||
                temp==(wchar_t *)"tanh" ||temp==(wchar_t *)"asin" ||temp==(wchar_t *)"acos" ||temp==(wchar_t *)"atan" ||
                temp==(wchar_t *)"abs" ||temp==(wchar_t *)"ceil" ||temp==(wchar_t *)"floor" ||temp==(wchar_t *)"sum" ||
                temp==(wchar_t *)"mult"||temp==(wchar_t *)"data" )
                {
                    formulaTokens.emplace_back(temp,1);
                    //If I want to add levels or types to my tokens, this will be handy
                }
                else //Here be the variables
                {
                    //This does the same thing as the other branch, but see comment in that branch
                    formulaTokens.emplace_back(temp,0);
                }
                i+=j-1;
            }
            else if(current=='=' || current=='!'||current=='>'||current=='<')
            {
                if(current=='!' && formula.at(i+1)=='=')
                {
                    formulaTokens.emplace_back(formula.substr(i,2),2);
                    ++i;
                }
                else
                {
                    formulaTokens.emplace_back(formula.substr(i,1),2);
                }
            }
            else if(current=='+' || current=='-')
            {formulaTokens.emplace_back(formula.substr(i,1),3);}
            else if(current=='*' || current=='/' || current=='%')
            {formulaTokens.emplace_back(formula.substr(i,1),4);}
            else if(current=='^')
            {
                formulaTokens.emplace_back(formula.substr(i,1),5);
            }
            else if(current=='(' || current==')' || current=='['|| current==']')
            {formulaTokens.emplace_back(formula.substr(i,1),6);}
            else if(current==',' || current==':')
            {formulaTokens.emplace_back(formula.substr(i,1),7);}
            else if(current==' ')
            {}
            else throw UnknownCharacter((wchar_t *)"Unknown Character : "+current);
        }
        return formulaTokens;
    }

        Node *Parser::Parse(std::vector<Token> &tokens) {
            Node* root= nullptr;
            parseFormula(root, tokens.begin());
            return root;
        }

        void Parser::parseFormula(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it) {
            Node* e2= nullptr;
            BinaryFunctions op = BUndefined;
            parseExpression(e,it);
            while(it->getValue()==(wchar_t *)"=" ||it->getValue()==(wchar_t *)"!=" ||it->getValue()==(wchar_t *)"<" ||it->getValue()==(wchar_t *)">")
            {
                if(it->getValue()==(wchar_t *)"=")
                {
                    op = Equals;

                }
                else if(it->getValue()==(wchar_t *)"!=")
                {
                    op = Unequals;

                }
                else if(it->getValue()==(wchar_t *)"<")
                {
                    op = Lesser;

                }
                else
                {
                    op = Greater;

                }
                ++it;
                parseExpression(e2,it);
                e= BinaryNode::BinaryNodeConstructor(e,e2,op);
            }
        }

        void Parser::parseExpression(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it) {
            Node* e2=nullptr;
            BinaryFunctions op = BUndefined;
            parseTerm(e,it);
            while(it->getValue()==(wchar_t *)"+" || it->getValue()==(wchar_t *)"-")
            {
                if(it->getValue()==(wchar_t *)"+")
                {
                    op = Plus;
                }
                else
                {
                    op = Minus;
                }
                ++it;
                parseTerm(e2,it);
                e = BinaryNode::BinaryNodeConstructor(e,e2,op);
            }
        }

        void Parser::parseTerm(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it) {
            Node* e2=nullptr;
            BinaryFunctions op = BUndefined;
            parseFactor(e,it);
            while(it->getValue()==(wchar_t *)"*" || it->getValue()==(wchar_t *)"/" || it->getValue()==(wchar_t *)"%")
            {
                if(it->getValue()==(wchar_t *)"*")
                {
                    op = Times;
                }
                else if(it->getValue()==(wchar_t *)"/")
                {
                    op = Divide;
                }
                else
                {
                    op = Modulo;
                }
                ++it;
                parseFactor(e2,it);
                e = BinaryNode::BinaryNodeConstructor(e,e2,op);
            }

        }

        void Parser::parseFactor(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it) {
            Node* e2 = nullptr;
            parsePossiblyNegatedOperand(e,it);
            while(it->getValue()==(wchar_t *)"^")
            {
                ++it;
                parsePossiblyNegatedOperand(e2,it);
                e= BinaryNode::BinaryNodeConstructor(e,e2,Exponent);
            }
        }

        void Parser::parsePossiblyNegatedOperand(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it) {
            bool isNegated = false;
            if(it->getValue()==(wchar_t *)"-")
            {
                isNegated=true;
                ++it;
            }
            parseOperand(e,it);
            if(isNegated)
            {
                e=UnaryNode::UnaryNodeConstructor(e,Negate);
            }

        }

        void Parser::parseOperand(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it) {
            if(it->getLevel()==0 || it->getValue()==(wchar_t *)"data")
            {
                parseQuantity(e,it);
            }
            else if(it->getValue()==(wchar_t *)"sum" || it->getValue()==(wchar_t *)"mult")
            {
                TernaryFunctions op = TUndefined;
                std::wstring countingVariable;
                Node* limit = nullptr;
                if(it->getValue()==(wchar_t *)"sum")
                {
                    op = Sum;
                }
                else
                {
                    op = Mult;
                }
                ++it;
                if(it->getValue()==(wchar_t *)"(")
                {}
                else
                    throw BadFormula((wchar_t *)"Missing left parentheses after ternary function");
                ++it;
                if (it->getLevel() == 0 && isdigit(it->getValue().at(0)))
                {}
                else
                    throw BadFormula((wchar_t *) "Number instead of variable in ternary function");
                countingVariable = it->getValue();
                ++it;
                if(it->getValue()==(wchar_t *)",")
                {}
                else
                    throw BadFormula((wchar_t *)"Missing comma between counting variable and limit expression");
                ++it;
                parseFormula(limit,it);
                ++it;
                if(it->getValue()==(wchar_t *)",")
                {}
                else
                    throw BadFormula((wchar_t *)"Missing comma between limit expression and evaluated expression");
                ++it;
                parseFormula(e,it);
                e = TernaryNode::TernaryNodeConstructor(countingVariable,limit,e,op);
            }
            else if(it->getLevel()==1)
            {}
            else
                throw BadFormula((wchar_t *)"Unexpected token");
        }

        void Parser::parseQuantity(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it) {

        }


        void Parser::parseData(Node *&e, std::iterator<std::vector<Token>,Token> &it) {

        }


    }}

