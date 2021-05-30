#ifndef ElaroSolutions_DARFormula_TESTS
#define ElaroSolutions_DARFormula_TESTS

#include "Formula.h"
#include <iostream>

using namespace ElaroSolutions::DARFormula;

int main(int argc, char const *argv[]) {
    Formula first;
    bool normalExceptionFlag= false;
    first.enableExceptionsOnCalculateValue();
    double value = NAN;

    std::cout << "----- Testing Integer Value -----" << std::endl;

    try{first.setFormula("10");}
    catch (BadFormula &b)
    {
        std::cout << b.what() <<std::endl;
        return 10;
    }
    value=NAN;
    try{value = first.calculateValue();}
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 11;
    }
    std::cout << value <<std::endl;

    try{first.setFormula("10a");}
    catch(BadFormula &b)
    {
        std::cout << b.what() << std::endl;
        normalExceptionFlag =true;
    }
    if(!normalExceptionFlag)
    {
        std::cout << first.formulaToText() << std::endl;
        return 12;
    }
    normalExceptionFlag=false;
    std::cout << "----- Testing Decimal Value -----" << std::endl;

    try{first.setFormula("56.56");}
    catch (std::exception& b)
    {
        std::cout << b.what() <<std::endl;
        return 20;
    }
    value = NAN;
    try{value = first.calculateValue();}
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 21;
    }
    std::cout << value <<std::endl;

    try{first.setFormula("56.a56");}
    catch(BadFormula &b)
    {
        std::cout << b.what() << std::endl;
        normalExceptionFlag =true;
    }
    if(!normalExceptionFlag)
    {
        std::cout << first.formulaToText() << std::endl;
        return 12;
    }
    normalExceptionFlag=false;

    std::cout << "----- Testing Random Variable -----" << std::endl;

    try{first.setFormula("r");}
    catch (std::exception& b)
    {
        std::cout << b.what() <<std::endl;
        return 30;
    }
    value = NAN;
    try{value = first.calculateValue();}
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 31;
    }
    std::cout << value <<std::endl;

    std::cout << "----- Testing Variable -----" << std::endl;
    Formula second;
    second.enableExceptionsOnCalculateValue();
    second.addVariable("Yay", 56.0);

    try{second.setFormula("Yay");}
    catch (std::exception& b)
    {
        std::cout << b.what() <<std::endl;
        return 40;
    }
    value = NAN;
    try{value = second.calculateValue();}
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 41;
    }
    std::cout << value <<std::endl;
    try {
        second.addOneToVariable("Yay");
        value = second.calculateValue();
    }catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 42;
    }
    std::cout << value <<std::endl;
    return 0;
}

#endif
