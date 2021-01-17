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

    Formula::Formula(IDataStructure *data, std::string formula)
    {
        Formula(data);
        setFormula(formula);
    }

    Formula::Formula(IDataStructure *data,std::string formula, std::string permittedVariables[])
    {
        Formula(data,formula);
        for(int i=0;;++i)
        {
            try
            {
                addVariable(permittedVariables[i],0.0);
            }
            catch(const std::out_of_range oor)
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
            if(typeid(*n)==typeid(VariableNode))
            {
                if(n->toString()=="r")
                {
                    return;
                }
                else
                {
                    for(int i=0;i<_allowedVariables.size();++i)
                    {
                        if(n->toString()==_allowedVariables.at(i))
                        {
                            return;
                        }
                    }
                    throw new UnexpectedVariable("Disallowed variable : "+(n->toString()));
                }
                
            }
            else if(typeid(*n)==typeid(DataNode))
            {
                DataNode* d = (DataNode*)n;
                for(int i=0;i<d->getIndexes().size();++i)
                {
                    checkNode(d->getIndexes().at(i));
                }
                return;
            }
        }
}

#endif