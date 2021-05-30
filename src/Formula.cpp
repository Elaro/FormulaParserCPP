#ifndef ElaroSolutions_DARFormula_FORMULA_CPP
#define ElaroSolutions_DARFormula_FORMULA_CPP


#include "Formula.h"

namespace ElaroSolutions { namespace DARFormula {


    Formula::Formula()
    {
        _data = nullptr;
        _root = nullptr;
        _throwsExceptionFromCalcValue = false;
        addVariable("E",2.718281828459045);
        addVariable("PI",3.141592653589793);
        addVariable("PHI",1.618033988749895);
    }

    Formula::Formula(std::string *permittedVariables)
    {
        _data = nullptr;
        _root = nullptr;
        _throwsExceptionFromCalcValue = false;
        addVariable("E",2.718281828459045);
        addVariable("PI",3.141592653589793);
        addVariable("PHI",1.618033988749895);
        for(int i=0;;i++)
        {
            try{
                addVariable(permittedVariables[i],0.0);
            } catch (std::out_of_range &oor)
            {
                break;
            }
        }
    }

    Formula::Formula(std::string permittedVariables[], std::string permittedFields[])
    {
        _data = nullptr;
        _root = nullptr;
        _throwsExceptionFromCalcValue = false;
        addVariable("E",2.718281828459045);
        addVariable("PI",3.141592653589793);
        addVariable("PHI",1.618033988749895);
        for(int i=0;;i++)
        {
            try{
                addVariable(permittedVariables[i],0.0);
            } catch (std::out_of_range &oor)
            {
                break;
            }
        }
        for(int i=0;;i++)
        {
            try{
                addField(permittedFields[i]);
            } catch (std::out_of_range &oor)
            {
                break;
            }
        }
    }

    void Formula::checkVariablesAndFields()
    {
            checkNode(_root);
    }

    void Formula::checkNode(Node *n)
    {
        if (typeid(*n) == typeid(VariableNode))
        {
            if (n->toText()=="r")
            {
                return;
            }
            else
            {
                if(_allowedVariables.find(n->toText())!=_allowedVariables.end())
                {
                   return;
                }
                throw UnexpectedVariable("Unsupported variable : " + (n->toText()));
            }
        }
        else if (typeid(*n) == typeid(DataNode))
        {
            auto *d = (DataNode *)n;
            for (unsigned int i = 0; i < d->getIndexes().size(); ++i)
            {
                checkNode(d->getIndexes().at(i));
            }
            if((_allowedFields.empty() && d->getField().empty())||_allowedFields.find(d->getField())!=_allowedFields.end())
            {
               return;
            }
            throw UnexpectedVariable("Unsupported field : " + d->getField());
        }
    }

    void Formula::setFormula(const std::string& formula)
    {

        delete _root;

        _root = Parser::Parse(Parser::Tokenize(formula),&_variables);

        checkVariablesAndFields();
    }

    void Formula::addVariable(const std::string& variableName, double initialValue)
    {
        if(_variables.count(variableName)>0)
        {
            updateVariable(variableName,initialValue);
            return;
        }
        _variables.emplace(variableName, initialValue);
        _allowedVariables.emplace(variableName);
    }

    double Formula::getVariableValue(const std::string& variableName)
    {
        return _variables.at(variableName);
    }

    void Formula::updateVariable(const std::string& variableName, double value)
    {
        _variables.at(variableName)=value;
    }

    void Formula::addOneToVariable(const std::string& variableName)
    {
        _variables.at(variableName)++;
    }

    void Formula::setData(IDataStructure *data) {
        _data =data;
    }

    void Formula::addField(const std::string& fieldName) {
        _allowedFields.insert(fieldName);
    }

    double Formula::calculateValue() {
        double result=0.0;
        try{
            result = _root->calcValue();
        } catch (std::exception &r)
        {
            if(_throwsExceptionFromCalcValue)
                throw r;
            else
                return NAN;
        }
        return result;
    }

    void Formula::enableExceptionsOnCalculateValue() {
        _throwsExceptionFromCalcValue = true;
    }

    void Formula::disableExceptionsOnCalculateValue() {
        _throwsExceptionFromCalcValue = false;
    }

    bool Formula::exceptionsOnCalculateValueEnabled() const {
        return _throwsExceptionFromCalcValue;
    }

    std::string Formula::formulaToText() {
        return _root->toText();
    }

    Formula::~Formula()
    {
        delete _root;

    }

    void Formula::limitDataIndexQuantity(int quantity)
    {
        limitDataNode(quantity,_root);

    }

    void Formula::limitDataNode(int quantity, Node *n)
    {
        if(n->getType()==Simple)
        {
            if(typeid(*n) == typeid(DataNode))
            {
                auto d = (DataNode *)n;
                if(d->getIndexes().size()<quantity)
                    throw BadFormula("Not enough bracketed formulas after data declaration");
                else
                    d->getIndexes().resize(quantity);
            }
            else return;
        }
        else if(n->getType()==Unary)
        {
            limitDataNode(quantity,((UnaryNode *)n)->getOperand());
        }
        else if(n->getType()==Binary)
        {
            limitDataNode(quantity,((BinaryNode *)n)->getPreOperand());
            limitDataNode(quantity,((BinaryNode *)n)->getPostOperand());
        }
        else if(n->getType()==Ternary)
        {
            limitDataNode(quantity,((TernaryNode *)n)->getLimit());
            limitDataNode(quantity,((TernaryNode *)n)->getFormula());
        }
    }




} }

#endif
