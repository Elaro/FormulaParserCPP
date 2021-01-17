#ifndef ElaroSolutions_DARFormula_FORMULA_CPP
#define ElaroSolutions_DARFormula_FORMULA_CPP


#include "Formula.h"

namespace ElaroSolutions::DARFormula {


    Formula::Formula()
    {
        _data = nullptr;
        _scanner = nullptr;
        _parser = nullptr;
        _root = nullptr;
        throwsExceptionFromCalcValue = false;
        addVariable("e",2.718281828459045);
        addVariable("pi",3.141592653589793);
        addVariable("phi",1.618033988749895);
    }

    Formula::Formula(IDataStructure *data)
    {
        Formula();
        _data = data;
    }

    Formula::Formula(IDataStructure *data, std::string permittedVariables[])
    {
        Formula(data);
        for(int i=0;;i++)
        {
            try{
                addVariable(permittedVariables[i],0.0);
            } catch (std::out_of_range oor)
            {
                break;
            }
        }
    }

    Formula::Formula(IDataStructure *data, std::string permittedVariables[], std::string permittedFields[])
    {
        Formula(data,permittedVariables);
        for(int i=0;;i++)
        {
            try{
                addField(permittedFields[i]);
            } catch (std::out_of_range oor)
            {
                break;
            }
        }
    }

    Formula::Formula(IDataStructure *data, std::string formula)
    {
        Formula(data);
        setFormula(formula);
    }

    Formula::Formula(IDataStructure *data,std::string formula, std::string permittedFields[])
    {
        Formula(data,formula);
        for(int i=0;;++i)
        {
            try
            {
                addField(permittedFields[i]);
            } catch (std::out_of_range oor)
            {
                break;
            }
        }
        checkVariablesAndFields();
    }

    void Formula::checkVariablesAndFields()
    {
        checkNode(_root);
    }

    void Formula::checkNode(Node *n)
    {
        if (typeid(*n) == typeid(VariableNode))
        {
            if (n->toString() == "r")
            {
                return;
            }
            else
            {
                if(_allowedVariables.find(n->toString())!=_allowedVariables.end())
                {
                   return;
                }
                throw new UnexpectedVariable("Unsupported variable : " + (n->toString()));
            }
        }
        else if (typeid(*n) == typeid(DataNode))
        {
            DataNode *d = (DataNode *)n;
            for (int i = 0; i < d->getIndexes().size(); ++i)
            {
                checkNode(d->getIndexes().at(i));
            }
            if((_allowedFields.size() == 0 && d->getField() == "")||_allowedFields.find(d->getField())!=_allowedFields.end())
            {
               return;
            }
            throw new UnexpectedVariable("Unsupported field : " + d->getField());
        }
    }

    std::string Formula::setFormula(std::string formula)
    {
        _scanner =new Scanner((const unsigned char *)formula.c_str(),(int)formula.size());
        _parser = new Parser(_scanner,&_variables);
        _root = _parser->Parse();
        if(_parser->errors->count>0)
        {
            _root = nullptr;
            return _parser->errors->summary;
        }
        return "";
    }

    void Formula::addVariable(std::string variableName, double initialValue)
    {
        if(_variables.count(variableName)>0)
        {
            updateVariable(variableName,initialValue);
            return;
        }
        _variables.emplace(variableName, initialValue);
        _allowedVariables.emplace(variableName);
    }

    double Formula::getVariableValue(std::string variableName)
    {
        return _variables.at(variableName);
    }

    void Formula::updateVariable(std::string variableName, double value)
    {
        _variables.insert_or_assign(variableName,value);
    }

    void Formula::addOneToVariable(std::string variableName)
    {
        _variables.insert_or_assign(variableName,_variables.at(variableName)+1);
    }
}

#endif
