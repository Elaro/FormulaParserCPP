#ifndef ElaroSolutions_DARFormula_FORMULA_CPP
#define ElaroSolutions_DARFormula_FORMULA_CPP


#include "Formula.h"

namespace ElaroSolutions { namespace DARFormula {


    Formula::Formula()
    {
        _data = nullptr;
        _scanner = nullptr;
        _parser = nullptr;
        _root = nullptr;
        throwsExceptionFromCalcValue = false;
        addVariable((wchar_t *)"E",2.718281828459045);
        addVariable((wchar_t *)"PI",3.141592653589793);
        addVariable((wchar_t *)"PHI",1.618033988749895);
    }

    Formula::Formula(IDataStructure *data)
    {
        _data = nullptr;
        _scanner = nullptr;
        _parser = nullptr;
        _root = nullptr;
        throwsExceptionFromCalcValue = false;
        addVariable((wchar_t *)"E",2.718281828459045);
        addVariable((wchar_t *)"PI",3.141592653589793);
        addVariable((wchar_t *)"PHI",1.618033988749895);
        _data = data;
    }

    Formula::Formula(IDataStructure *data, std::wstring permittedVariables[])
    {
        _data = nullptr;
        _scanner = nullptr;
        _parser = nullptr;
        _root = nullptr;
        throwsExceptionFromCalcValue = false;
        addVariable((wchar_t *)"E",2.718281828459045);
        addVariable((wchar_t *)"PI",3.141592653589793);
        addVariable((wchar_t *)"PHI",1.618033988749895);
        _data = data;
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

    Formula::Formula(IDataStructure *data, std::wstring permittedVariables[], std::wstring permittedFields[])
    {
        _data = nullptr;
        _scanner = nullptr;
        _parser = nullptr;
        _root = nullptr;
        throwsExceptionFromCalcValue = false;
        addVariable((wchar_t *)"E",2.718281828459045);
        addVariable((wchar_t *)"PI",3.141592653589793);
        addVariable((wchar_t *)"PHI",1.618033988749895);
        _data = data;
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
            if (n->toText().compare((wchar_t *)"r")==0)
            {
                return;
            }
            else
            {
                if(_allowedVariables.find(n->toText())!=_allowedVariables.end())
                {
                   return;
                }
                throw UnexpectedVariable((wchar_t *)"Unsupported variable : " + (n->toText()));
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
            throw UnexpectedVariable((wchar_t *)"Unsupported field : " + d->getField());
        }
    }

    std::wstring Formula::setFormula(const std::wstring& formula)
    {
        _scanner =new Scanner((const unsigned char *)formula.c_str(),(int)formula.size());
        _parser = new Parser(_scanner,&_variables);
        _root = _parser->Parse();
        if(_parser->errors->count>0)
        {
            _root = nullptr;
            return _parser->errors->summary;
        }
        return std::wstring((wchar_t *)"");
    }

    void Formula::addVariable(const std::wstring& variableName, double initialValue)
    {
        if(_variables.count(variableName)>0)
        {
            updateVariable(variableName,initialValue);
            return;
        }
        _variables.emplace(variableName, initialValue);
        _allowedVariables.emplace(variableName);
    }

    double Formula::getVariableValue(const std::wstring& variableName)
    {
        return _variables.at(variableName);
    }

    void Formula::updateVariable(const std::wstring& variableName, double value)
    {
        _variables.at(variableName)=value;
    }

    void Formula::addOneToVariable(const std::wstring& variableName)
    {
        _variables.at(variableName)++;
    }

    void Formula::setData(IDataStructure *data) {
        _data =data;
    }

    void Formula::addField(std::wstring fieldName) {

    }

    double Formula::calculateValue() {
        return 0;
    }

    void Formula::enableExceptionsOnCalculateValue() {
        throwsExceptionFromCalcValue = true;
    }

    void Formula::disableExceptionsOnCalculateValue() {
        throwsExceptionFromCalcValue = false;
    }

    bool Formula::exceptionsOnCalculateValueEnabled() const {
        return throwsExceptionFromCalcValue;
    }

    std::wstring Formula::formulaToText() {
        return _root->toText();
    }
} }

#endif
