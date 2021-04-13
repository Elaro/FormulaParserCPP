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

    std::wstring ValueNode::toText()
    {
        return std::to_wstring(_value);
    }

    VariableNode::VariableNode(std::wstring variable, std::unordered_map<std::wstring, double> *variables)
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
                std::wstring error= std::wstring().append((wchar_t *)"Variable ").append(_variable).
                        append((wchar_t *)" has no value");
                throw UninitializedVariable(error);
            }

        return value;
    }

    std::wstring VariableNode::toText()
    {
        return _variable;
    }

    VariableNode::~VariableNode()
    {
        _variables = nullptr;
    }

    DataNode::DataNode(std::vector<ElaroSolutions::DARFormula::Node*> indexes, std::wstring field)
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

    std::wstring DataNode::toText()
    {
        std::wstring result=(wchar_t *)"data";
        for(auto & _indexe : _indexes)
        {
            result.append((wchar_t  *)"["+_indexe->toText()+(wchar_t *)"]");
        }
        if(!_field.empty())
        {
            result.append((wchar_t *)":"+_field);
        }
        return result;
    }

    std::vector<ElaroSolutions::DARFormula::Node*> DataNode::getIndexes()
    {
        return _indexes;
    }

    std::wstring DataNode::getField()
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

    Node *BinaryNode::BinaryNodeConstructor(Node *preoperand, Node *postoperand, BinaryFunctions op) {
        return nullptr;
    }

    NodeType BinaryNode::getType() {
        return Binary;
    }

    NodeType TernaryNode::getType() {
        return Ternary;
    }

    Node *TernaryNode::TernaryNodeConstructor(const std::wstring& countingVariable, Node *limit, Node *formula, TernaryFunctions op) {
        return nullptr;
    }

        std::wstring RandomVariableNode::toText() {
            return std::wstring((wchar_t *)"r");
        }

        double RandomVariableNode::calcValue() {
            return _rng->generateNumber();
        }

        RandomVariableNode::RandomVariableNode() {
            _rng = JavalikeRandomNumberGenerator::getJRNG();
        }
    } }

#endif
