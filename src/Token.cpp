//
// Created by fldenhezt on 21-03-07.
//

#include "Token.h"

ElaroSolutions::DARFormula::Token::Token(std::wstring value, int level) {
    _value = value;
    _level = level;
}

const std::wstring &ElaroSolutions::DARFormula::Token::getValue() const {
    return _value;
}

const int ElaroSolutions::DARFormula::Token::getLevel() const {
    return _level;
}
