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

    Node *UnaryNode::UnaryNodeConstructor(Node *operand, UnaryFunctions op)
    {
        UnaryNode * construct= nullptr;
        switch(op)
        {
            case Sin: construct = new SinNode(operand);
                break;
            case Cos: construct = new CosNode(operand);
                break;
            case Tan: construct = new TanNode(operand);
                break;
            case Asin: construct = new AsinNode(operand);
                break;
            case Acos: construct = new AcosNode(operand);
                break;
            case Atan: construct = new AtanNode(operand);
                break;
            case Sinh: construct = new SinhNode(operand);
                break;
            case Cosh: construct = new CoshNode(operand);
                break;
            case Tanh: construct = new TanhNode(operand);
                break;
            case Log:  construct = new LogNode(operand);
                break;
            case Ln:  construct = new LnNode(operand);
                break;
            case Sqrt: construct = new SqrtNode(operand);
                break;
            case Ceil: construct = new CeilNode(operand);
                break;
            case Floor: construct = new FloorNode(operand);
                break;
            case Abs:  construct = new AbsNode(operand);
                break;
            case Negate:  construct = new NegateNode(operand);
                break;
            case UUndefined: throw std::runtime_error("Unsupported Unary Node construction");
                break;
        }
        return construct;
    }

        Node *UnaryNode::getOperand()
        {
            return _operand;
        }

        UnaryNode::UnaryNode(Node *operand)
        {
            _operand = operand;
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

        double SinNode::calcValue()
        {
            return sin(_operand->calcValue());
        }

        std::string SinNode::toText()
        {
            return std::__cxx11::string("sin("+_operand->toText()+")");
        }

        double CosNode::calcValue()
        {
            return cos(_operand->calcValue());
        }

        std::string CosNode::toText()
        {
            return std::__cxx11::string("cos("+_operand->toText()+")");
        }

        double TanNode::calcValue()
        {
            return tan(_operand->calcValue());
        }

        std::string TanNode::toText()
        {
            return std::__cxx11::string("tan("+_operand->toText()+")");
        }

        double AsinNode::calcValue()
        {
            return asin(_operand->calcValue());
        }

        std::string AsinNode::toText()
        {
            return std::__cxx11::string("asin("+_operand->toText()+")");
        }

        double AcosNode::calcValue()
        {
            return acos(_operand->calcValue());
        }

        std::string AcosNode::toText()
        {
            return std::__cxx11::string("acos("+_operand->toText()+")");
        }

        double AtanNode::calcValue()
        {
            return atan(_operand->calcValue());
        }

        std::string AtanNode::toText()
        {
            return std::__cxx11::string("atan("+_operand->toText()+")");
        }

        double SinhNode::calcValue()
        {
            return sinh(_operand->calcValue());
        }

        std::string SinhNode::toText()
        {
            return std::__cxx11::string("sinh("+_operand->toText()+")");
        }

        double CoshNode::calcValue()
        {
            return cosh(_operand->calcValue());
        }

        std::string CoshNode::toText()
        {
            return std::__cxx11::string("cosh("+_operand->toText()+")");
        }

        double TanhNode::calcValue()
        {
            return tanh(_operand->calcValue());
        }

        std::string TanhNode::toText()
        {
            return std::__cxx11::string("tanh("+_operand->toText()+")");
        }

        double LogNode::calcValue()
        {
            return log10(_operand->calcValue());
        }

        std::string LogNode::toText()
        {
            return std::__cxx11::string("log("+_operand->toText()+")");
        }

        double LnNode::calcValue()
        {
            return log(_operand->calcValue());
        }

        std::string LnNode::toText()
        {
            return std::__cxx11::string("ln("+_operand->toText()+")");
        }

        double SqrtNode::calcValue()
        {
            return sqrt(_operand->calcValue());
        }

        std::string SqrtNode::toText()
        {
            return std::__cxx11::string("sqrt("+_operand->toText()+")");
        }

        double CeilNode::calcValue()
        {
            return ceil(_operand->calcValue());
        }

        std::string CeilNode::toText()
        {
            return std::__cxx11::string("ceil("+_operand->toText()+")");
        }

        double FloorNode::calcValue()
        {
            return floor(_operand->calcValue());
        }

        std::string FloorNode::toText()
        {
            return std::__cxx11::string("floor("+_operand->toText()+")");
        }

        double AbsNode::calcValue()
        {
            return fabs(_operand->calcValue());
        }

        std::string AbsNode::toText()
        {
            return std::__cxx11::string("abs("+_operand->toText()+")");
        }

        double NegateNode::calcValue()
        {
            return -_operand->calcValue();
        }

        std::string NegateNode::toText()
        {
            return std::__cxx11::string("-("+_operand->toText()+")");
        }
    } }

#endif
