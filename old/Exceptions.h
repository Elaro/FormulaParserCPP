//
// Created by dev on 2016-04-05.
//

#ifndef MATHEMODEON_ELAROEXCEPTIONS_H
#define MATHEMODEON_ELAROEXCEPTIONS_H



#include <stdexcept>

namespace ElaroDARFormula {

    class UnxpctdVarException : std::runtime_error
    {
    public:
        UnxpctdVarException(const std::string message) : std::runtime_error(message){};

    };

    class UnxpctdTknException : std::runtime_error
    {
    public:
        UnxpctdTknException(std::string message) : std::runtime_error(message){};
    };

    class UnxpctdCharException : std::runtime_error
    {
    public:
        UnxpctdCharException(std::string message) : std::runtime_error(message){};
    };

    class UnevnParenException : std::runtime_error
    {
    public:
        UnevnParenException(std::string message) : std::runtime_error(message){};
    };

    class UnxpctdEOLException:std::runtime_error
    {
    public:
        UnxpctdEOLException(std::string message) : std::runtime_error(message){};
    };
}


#endif //MATHEMODEON_ELAROEXCEPTIONS_H
