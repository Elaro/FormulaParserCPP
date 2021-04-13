//
// Created by fldenhezt on 21-03-07.
//

#ifndef RUNTIMEFORMULA_IN_C____GCC__TOKEN_H
#define RUNTIMEFORMULA_IN_C____GCC__TOKEN_H

#include <string>
#include <cwchar>
#include <map>



namespace ElaroSolutions{
    namespace DARFormula{

        class Token {
            int _level;
            std::wstring _value;
        public:
            explicit Token(std::wstring value, int level);
            const std::wstring &getValue() const;
            const int getLevel() const;
        };




    }
}



#endif //RUNTIMEFORMULA_IN_C____GCC__TOKEN_H