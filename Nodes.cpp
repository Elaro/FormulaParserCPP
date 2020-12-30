#ifndef ElaroSolutions_DARFormula_NODES_CPP
#define ElaroSolutions_DARFormula_NODES_CPP

#include "Nodes.h"

namespace ElaroSolutions::DARFormula {
   /* std::unordered_map<std::string, double> Node::getCurrentVariables()
    {
    }*/

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

    VariableNode::VariableNode(std::string variable)
    {
        _variable = variable;
    }

    void VariableNode::setVariables(std::unordered_map<std::string,double> *variables)
    {
        _variables = variables;
    }

    double VariableNode::calcValue()
    {
        double value;
        if(_variable == "r")
        {
            value = 
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

    DataNode::DataNode(std::vector<ElaroSolutions::DARFormula::Node> indexes, std::string field)
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
        
    }
}

#endif