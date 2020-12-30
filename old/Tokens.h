//
// Created by dev on 2016-04-03.
//

#ifndef MATHEMODEON_TOKENS_H
#define MATHEMODEON_TOKENS_H

#include <string>
#include <vector>
#include <regex>
#include <stack>
#include <iostream>

#include "Exceptions.h"


namespace ElaroDARFormula
{
    std::string stringToLower(std::string target);

    class Token {

    private:
        std::string _element;
        int _level;
    public:
        Token(std::string element, int level);
        Token(char element, int level);
        Token(const Token &that);
        ~Token();

        const std::string toString();
        const std::string getElement();
        int getLevel();
        void setElement(const std::string &element);
        void setLevel(int level);
        bool operator==(Token that);
    };

    class FormulaTokens:public std::vector<Token>
    {
        void expect(Token testee, Token expected);
        vector<Token>::iterator s(std::vector<Token>::iterator it);
        void p(std::vector<Token>::iterator it);

    public:
        FormulaTokens();
		~FormulaTokens();

        static FormulaTokens* Tokenize(std::string expression);
        FormulaTokens* checkFormula();
        FormulaTokens* checkVariables(std::vector<std::string> expectedVars);
        FormulaTokens* makePostfix();
        std::string toString();
    };
}
#endif //MATHEMODEON_TOKENS_H
