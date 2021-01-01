#ifndef ElaroSolutions_DARFormula_NODES_H
#define ElaroSolutions_DARFormula_NODES_H



namespace ElaroSolutions::DARFormula {

#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <cmath>

    enum NodeType
    {
        Simple, Unary, Binary, Ternary
    };

    enum UnaryFunctions
    {
        Sin=3, Cos, Tan, Asin, Acos, Atan, Sinh, Cosh, Tanh, Log, Ln,  Sqrt, Ceil, Floor, Abs,  Negate, Undefined
    };

    enum BinaryFunctions
    {
        Equals,Unequals,Greater,Lesser,Plus,Minus,Times,Divide,Modulo,Exponent, Undefined

    };

    enum TernaryFunctions
    {
        Sum=18, Mult, Undefined
    };

    class IDataStructure
    {
        public:
        virtual double getValueAt(int indexes[], std::string field)=0;
    };

    class JavalikeRandomNumberGenerator
    {
        static std::default_random_engine _r;

        public:
        JavalikeRandomNumberGenerator(); 
        double generateNumber(); // Generates a number in the range [0,1[
    };

    class Node
    {
        public:
        virtual double calcValue()=0;
        virtual std::string toString()=0;
        virtual NodeType getType()=0;
      //  std::unordered_map<std::string,double> getCurrentVariables();
        virtual ~Node();
    };

    class SimpleNode : Node
    {
        public:
        virtual double calcValue()=0;
        virtual std::string toString()=0;
        NodeType getType();
        virtual ~SimpleNode();
    };

    class ValueNode : SimpleNode
    {
        double _value;

        public:
        ValueNode(double value);
        double calcValue();
        std::string toString();
    };

    class VariableNode : SimpleNode
    {
        std::unordered_map<std::string, double> *_variables;
        static JavalikeRandomNumberGenerator _rng;
        std::string _variable;

        public:
        VariableNode(std::string variable, std::unordered_map<std::string, double> *variables);
        double calcValue();
        std::string toString();

        ~VariableNode();
    };

    class DataNode : SimpleNode
    {
        static IDataStructure *_data;
        std::vector<ElaroSolutions::DARFormula::Node*> _indexes;
        std::string _field;

        public:
        DataNode(std::vector<ElaroSolutions::DARFormula::Node*> indexes, std::string field);
        double calcValue();
        std::string toString();

        static void setData(IDataStructure *data);
        ~DataNode();
    };
}

#endif
