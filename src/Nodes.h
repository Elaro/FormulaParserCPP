#ifndef ElaroSolutions_DARFormula_NODES_H
#define ElaroSolutions_DARFormula_NODES_H

#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <cmath>
#include <stdexcept>
#include <ctime>

#include "IDataStructure.h"
#include "Exceptions.h"


#define PRECISION 0.000005

namespace ElaroSolutions { namespace DARFormula
    {

        enum NodeType
        {
            Simple, Unary, Binary, Ternary
        };

        enum UnaryFunctions
        {
            Sin, Cos, Tan, Asin, Acos, Atan, Sinh, Cosh, Tanh, Log, Ln, Sqrt, Ceil, Floor, Abs, Negate, UUndefined
        };

        enum BinaryFunctions
        {
            Equals, Unequals, Greater, Lesser, Plus, Minus, Times, Divide, Modulo, Exponent, BUndefined
        };

        enum TernaryFunctions
        {
            Sum, Mult, TUndefined
        };

        class JavalikeRandomNumberGenerator : std::default_random_engine
        {
            static JavalikeRandomNumberGenerator *_r;

            explicit JavalikeRandomNumberGenerator() : std::default_random_engine(time(nullptr))
            {}

        public:
            static JavalikeRandomNumberGenerator *getJRNG();

            static double generateNumber(); // Generates a number in the range [0,1[
        };

        class Node
        {
        public:
            virtual double calcValue() = 0;

            virtual std::string toText() = 0;

            virtual NodeType getType() = 0;

            //  std::unordered_map<std::string,double> getCurrentVariables();
            virtual ~Node() = 0;
        };

        class SimpleNode : public Node
        {
        public:
            NodeType getType() override;

            ~SimpleNode() override = 0;
        };

        class ValueNode : public SimpleNode
        {
            double _value;

        public:
            explicit ValueNode(double value);

            double calcValue() override;

            std::string toText() override;
        };

        class VariableNode : public SimpleNode
        {
            std::unordered_map<std::string, double> *_variables;
            std::string _variable;

        public:
            VariableNode(std::string variable, std::unordered_map<std::string, double> *variables);

            double calcValue() override;

            std::string toText() override;

            ~VariableNode() override;
        };

        class RandomVariableNode : public SimpleNode
        {
            JavalikeRandomNumberGenerator *_rng;

        public:
            RandomVariableNode();

            double calcValue() override;

            std::string toText() override;
        };

        class DataNode : public SimpleNode
        {
            IDataStructure *_data{};
            std::vector<ElaroSolutions::DARFormula::Node *> _indexes;
            int *_indexTable;
            std::string _field;

        public:
            DataNode(std::vector<ElaroSolutions::DARFormula::Node *> indexes, std::string field);

            double calcValue() override;

            std::string toText() override;

            std::vector<ElaroSolutions::DARFormula::Node *> getIndexes();

            std::string getField();

            void setData(IDataStructure *data);

            ~DataNode() override;
        };

        class UnaryNode : public Node
        {
        protected:
            Node *_operand;
        public:
            explicit UnaryNode(Node* operand);
            NodeType getType() override;

            Node *getOperand();

            static Node *UnaryNodeConstructor(Node *operand, UnaryFunctions op);
        };

        class SinNode : public UnaryNode
        {
        public:
            explicit SinNode(Node *operand) : UnaryNode(operand){}

            double calcValue() override;

            std::string toText() override;
        };

        class CosNode : public UnaryNode
        {
        public:
            explicit CosNode(Node *operand) : UnaryNode(operand){}

            double calcValue() override;

            std::string toText() override;

        };

        class TanNode : public UnaryNode
        {
        public:
            explicit TanNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;
        };

        class AsinNode : public UnaryNode
        {
        public:
            explicit AsinNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;
        };

        class AcosNode : public UnaryNode
        {
        public:
            explicit AcosNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class AtanNode : public UnaryNode
        {
        public:
            explicit AtanNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;
        };

        class SinhNode : public UnaryNode
        {
        public:
            explicit SinhNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;
        };

        class CoshNode : public UnaryNode
        {
        public:
            explicit CoshNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;
        };

        class TanhNode : public UnaryNode
        {
        public:
            explicit TanhNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class LogNode : public UnaryNode
        {
        public:
            explicit LogNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class LnNode : public UnaryNode
        {
        public:
            explicit LnNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class SqrtNode : public UnaryNode
        {
        public:
            explicit SqrtNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class CeilNode : public UnaryNode
        {
        public:
            explicit CeilNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class FloorNode : public UnaryNode
        {
        public:
            explicit FloorNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class AbsNode : public UnaryNode
        {
        public:
            explicit AbsNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

        class NegateNode : public UnaryNode
        {
        public:
            explicit NegateNode(Node *operand) : UnaryNode(operand){}
            double calcValue() override;

            std::string toText() override;

        };

    class BinaryNode : public Node
    {
        Node *_preoperand, *_postoperand;

        public:
        NodeType getType() override;
        Node * getPreOperand();
        Node * getPostOperand();
        static Node* BinaryNodeConstructor(Node *preoperand,Node *postoperand, BinaryFunctions op);
    };

    class TernaryNode : public Node
    {
        VariableNode *_counter;
        Node *_limit, *_formula;
        public:
        NodeType getType() override;
        VariableNode * getCounter();
        Node * getLimit();
        Node * getFormula();
        static Node* TernaryNodeConstructor(const std::string& countingVariable,Node *limit, Node *formula, TernaryFunctions op, std::unordered_map<std::string, double> *variables);
    };
} }

#endif
