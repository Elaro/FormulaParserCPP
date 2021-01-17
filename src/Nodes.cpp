#ifndef ElaroSolutions_DARFormula_NODES_CPP
#define ElaroSolutions_DARFormula_NODES_CPP

#include "Nodes.h"

namespace ElaroSolutions::DARFormula {
   /* std::unordered_map<std::string, double> Node::getCurrentVariables()
    {
    }*/

    std::default_random_engine * JavalikeRandomNumberGenerator::_r = nullptr;

    JavalikeRandomNumberGenerator::JavalikeRandomNumberGenerator()
    {
        if(_r==nullptr)
        {
            _r = new std::default_random_engine(time(NULL));
        }
    }

    double JavalikeRandomNumberGenerator::generateNumber()
    {
        return (double)(*_r)()/(double)_r->max();
    }

    NodeType SimpleNode::getType()
    {
        return Simple;
    }

    ValueNode::ValueNode(double value)
    {
        _value = value;
    }

    double ValueNode::calcValue()
    {
        return _value;
    }

    std::string ValueNode::toString()
    {
        return std::to_string(_value);
    }

    VariableNode::VariableNode(std::string variable, std::unordered_map<std::string, double> *variables)
    {
        _variable = variable;
        _variables = variables;
        _rng = JavalikeRandomNumberGenerator::getJRNG();
    }

    double VariableNode::calcValue()
    {
        double value=NAN;
        if(_variable == "r")
        {
            value = _rng->generateNumber();
        }
        else
        {
            try
            {
                value = _variables->at(_variable);
            }
            catch(std::out_of_range oor)
            {
                throw new UninitializedVariable("Variable "+_variable+" has no value");
            }
        }
        
        return value;
    }

    std::string VariableNode::toString()
    {
        return _variable;
    }

    VariableNode::~VariableNode()
    {
        _variables = nullptr;
    }

    DataNode::DataNode(std::vector<ElaroSolutions::DARFormula::Node*> indexes, std::string field)
    {
        _indexes = indexes;
        _field = field;
    }

    void DataNode::setData(IDataStructure *data)
    {
        _data = data;
    }

    double DataNode::calcValue()
    {
        unsigned int indexQuantity = _indexes.size();
        int *indexAt = new int[indexQuantity];
        for(int i=0;i<indexQuantity;++i)
        {
            indexAt[i]=_indexes.at(i)->calcValue();
        }
        return _data->getValueAt(indexAt,_field);
    }

    std::string DataNode::toString()
    {
        std::string result="data";
        for(int i=0;i<_indexes.size();++i)
        {
            result.append("["+_indexes.at(i)->toString()+"]");
        }
        if(_field!="")
        {
            result.append(":"+_field);
        }
        return result;
    }

    std::vector<ElaroSolutions::DARFormula::Node*> DataNode::getIndexes()
    {
        return _indexes;
    }

    std::string DataNode::getField()
    {
        return _field;
    }

    void DataNode::setData(IDataStructure *data)
    {
        _data = data;
    }

    DataNode::~DataNode()
    {
        for(int i=0;i<_indexes.size();++i)
        {
            delete _indexes.at(i);
        }
    }
}

#endif