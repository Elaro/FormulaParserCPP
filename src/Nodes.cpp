#ifndef ElaroSolutions_DARFormula_NODES_CPP
#define ElaroSolutions_DARFormula_NODES_CPP

#include "Nodes.h"

#include <utility>

namespace ElaroSolutions { namespace DARFormula {
   /* std::unordered_map<std::string, double> Node::getCurrentVariables()
    {
    }*/

    JavalikeRandomNumberGenerator * JavalikeRandomNumberGenerator::_r = nullptr;

    double JavalikeRandomNumberGenerator::generateNumber()
    {
        return (double)(*getJRNG())()/ (double) (JavalikeRandomNumberGenerator::max()+1.0);
    }

    JavalikeRandomNumberGenerator *JavalikeRandomNumberGenerator::getJRNG() {
        if(_r == nullptr)
        {
            _r = new JavalikeRandomNumberGenerator();
        }
        return _r;
    }

    Node::~Node() = default;

    NodeType SimpleNode::getType()
    {
        return Simple;
    }

    SimpleNode::~SimpleNode() = default;

    ValueNode::ValueNode(double value)
    {
        _value = value;
    }

    double ValueNode::calcValue()
    {
        return _value;
    }

    std::string ValueNode::toText()
    {
        return std::to_string(_value);
    }

    VariableNode::VariableNode(std::string variable, std::unordered_map<std::string, double> *variables)
    {
        _variable = std::move(variable);
        _variables = variables;
    }

    double VariableNode::calcValue()
    {
        double value=NAN;
            try
            {
                value = _variables->at(_variable);
            }
            catch(std::out_of_range&)
            {
                std::string error= std::string().append("Variable ").append(_variable).
                        append(" has no value");
                throw UninitializedVariable(error);
            }

        return value;
    }

    std::string VariableNode::toText()
    {
        return _variable;
    }

    VariableNode::~VariableNode()
    {
        _variables = nullptr;
    }

    DataNode::DataNode(std::vector<ElaroSolutions::DARFormula::Node*> indexes, std::string field)
    {
        _indexes = std::move(indexes);
        _field = std::move(field);
        _indexTable =new int[_indexes.size()];
    }

    void DataNode::setData(IDataStructure *data)
    {
        _data = data;
    }

    double DataNode::calcValue()
    {
        unsigned int indexQuantity = _indexes.size();

        for(unsigned int i=0;i<indexQuantity;++i)
        {
            _indexTable[i]=(int)std::round(_indexes.at(i)->calcValue());
        }
        double value=_data->getValueAt(_indexTable,_field);

        return value;
    }

    std::string DataNode::toText()
    {
        std::string result="data";
        for(auto & _indexe : _indexes)
        {
            result.append("["+_indexe->toText()+"]");
        }
        if(!_field.empty())
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

    DataNode::~DataNode()
    {
        for(auto & _index : _indexes)
        {
            delete _index;
        }
        delete[] _indexTable;
    }

    NodeType UnaryNode::getType() {
        return Unary;
    }

    Node *UnaryNode::UnaryNodeConstructor(Node *operand, UnaryFunctions op) {
        return nullptr;
    }

        Node *UnaryNode::getOperand()
        {
            return _operand;
        }

        Node *BinaryNode::BinaryNodeConstructor(Node *preoperand, Node *postoperand, BinaryFunctions op) {
        return nullptr;
    }

    NodeType BinaryNode::getType() {
        return Binary;
    }

        Node *BinaryNode::getPreOperand()
        {
            return _preoperand;
        }

        Node *BinaryNode::getPostOperand()
        {
            return _postoperand;
        }

        NodeType TernaryNode::getType() {
        return Ternary;
    }

    Node *TernaryNode::TernaryNodeConstructor(const std::string& countingVariable, Node *limit, Node *formula, TernaryFunctions op, std::unordered_map<std::string,double> *variables) {
        return nullptr;
    }

    VariableNode *TernaryNode::getCounter()
    {
        return _counter;
    }

        Node *TernaryNode::getLimit()
        {
            return _limit;
        }

        Node *TernaryNode::getFormula()
        {
            return _formula;
        }

        std::string RandomVariableNode::toText() {
        return std::string("r");
    }

    double RandomVariableNode::calcValue() {
        return _rng->generateNumber();
    }

    RandomVariableNode::RandomVariableNode() {
        _rng = JavalikeRandomNumberGenerator::getJRNG();
    }
    } }

#endif
