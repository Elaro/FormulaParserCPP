%module(directors="1") formula
%{
#include "Exceptions.h"
#include "IDataStructure.h"
#include "Token.h"
#include "Parser.h"
#include "Nodes.h"
#include "Formula.h"
%}

#define PRECISION 0.000005

%feature("director");

class Formula
{
    public:
    Formula();
    ~Formula();
    void setUpFormula(std::string* allowedVariables, std::string* allowedFields, IDataStructure* data);
    void setFormula(const std::string& formula);
    void addVariable(const std::string& variableName, double initialValue);
    double getVariableValue(const std::string& variableName);
    void updateVariable(const std::string& variableName, double value);
    void addOneToVariable(const std::string& variableName);
    void limitDataIndexQuantity(int quantity);
    void addField(const std::string& fieldName);
    double calculateValue();
    void enableExceptionsOnCalculateValue();
    void disableExceptionsOnCalculateValue();
    bool exceptionsOnCalculateValueEnabled() const;
    std::string formulaToText();
    
};

class IDataStructure
{
    public:
    virtual double getValueAt(int indexes[], std::string field)=0;
};