#ifndef ElaroSolutions_DARFormula_TESTS
#define ElaroSolutions_DARFormula_TESTS

#include "Formula.h"
#include <iostream>

using namespace ElaroSolutions::DARFormula;

class MockData : public IDataStructure
{
    std::vector<std::vector<double>> _mock;
public:
    MockData() = default;

    double getValueAt(int indexes[], std::string field) override
    {
        try{
            return _mock.at(indexes[0]).at(indexes[1]);
        }
        catch(std::out_of_range &oor)
        {
            return NAN;
        }
    };
    std::vector<std::vector<double>>* getMock()
    {
        return &_mock;
    };
};

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

    std::cout << "----- Testing Data -----" << std::endl;
    Formula third;
    MockData data;
    std::vector<double> vec1;
    data.getMock()->push_back(vec1);
    data.getMock()->at(0).push_back(56.56);
    third.setUpFormula(nullptr,nullptr,&data);
    third.setFormula("data[0][0][0]");
    third.limitDataIndexQuantity(2);
    value=-1.0;
    try{
        value = third.calculateValue();
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 50;
    }
    std::cout << value << std::endl;

    return 0;
}

#endif
