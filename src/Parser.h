//
// Created by fldenhezt on 21-03-04.
//

#ifndef RUNTIMEFORMULA_IN_C____GCC__PARSER_H
#define RUNTIMEFORMULA_IN_C____GCC__PARSER_H

#include <vector>
#include <iterator>
#include "Token.h"
#include "Nodes.h"

namespace ElaroSolutions{namespace DARFormula{

    class Parser {
        static void parseFormula(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseExpression(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseTerm(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseFactor(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parsePossiblyNegatedOperand(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseOperand(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseQuantity(Node *&e, __gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseFunc(UnaryFunctions &op,__gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseFunc3(TernaryFunctions &op,__gnu_cxx::__normal_iterator<Token *, std::vector<Token, std::allocator<Token>>> it);
        static void parseData(Node *&e, std::iterator<std::vector<Token>,Token> &it);
    public:
        static std::vector<Token> Tokenize(std::wstring &formula);
        static Node* Parse(std::vector<Token> &tokens);
    };

}}



#endif //RUNTIMEFORMULA_IN_C____GCC__PARSER_H
