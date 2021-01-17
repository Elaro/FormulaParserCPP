#ifndef ElaroSolutions_DARFormula_NODES_H
#define ElaroSolutions_DARFormula_NODES_H

#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <cmath>
#include <stdexcept>
#include <time.h>

#include "IDataStructure.h"
#include "Exceptions.h"


#define PRECISION 0.000005

namespace ElaroSolutions::DARFormula {

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

    class JavalikeRandomNumberGenerator
    {
        static std::default_random_engine *_r;

        protected:
        JavalikeRandomNumberGenerator(); 

        public:
        static JavalikeRandomNumberGenerator * getJRNG();
        static double generateNumber(); // Generates a number in the range [0,1[
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

    class SimpleNode : public Node
    {
        public:
        NodeType getType();
        virtual ~SimpleNode();
    };

    class ValueNode : public SimpleNode
    {
        double _value;

        public:
        ValueNode(double value);
        double calcValue();
        std::string toString();
    };

    class VariableNode : public SimpleNode
    {
        std::unordered_map<std::string, double> *_variables;
        static JavalikeRandomNumberGenerator * _rng;
        std::string _variable;

        public:
        VariableNode(std::string variable, std::unordered_map<std::string, double> *variables);
        double calcValue();
        std::string toString();

        ~VariableNode();
    };

    class DataNode : public SimpleNode
    {
        static IDataStructure *_data;
        std::vector<ElaroSolutions::DARFormula::Node*> _indexes;
        std::string _field;

        public:
        DataNode(std::vector<ElaroSolutions::DARFormula::Node*> indexes, std::string field);
        double calcValue();
        std::string toString();
        std::vector<ElaroSolutions::DARFormula::Node*> getIndexes();
        std::string getField();

        static void setData(IDataStructure *data);
        ~DataNode();
    };

    class UnaryNode : public Node
    {
        public:
        NodeType getType();
        static Node* UnaryNodeConstructor(Node *operand,UnaryFunctions op);
    };

    class BinaryNode : public Node
    {
        public:
        NodeType getType();
        static Node* BinaryNodeConstructor(Node *preoperand,Node *postoperand, BinaryFunctions op);
    };

    class TernaryNode : public Node
    {
        public:
        NodeType getType();
        static Node* TernaryNodeConstructor(std::string countingVariable,Node *limit, Node *formula, TernaryFunctions op);
    };
}

#endif
