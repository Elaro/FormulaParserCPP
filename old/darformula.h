//// Created by dev on 2/27/2016.//
#ifndef ELARO_DARFORMULA_H
#define ELARO_DARFORMULA_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <unordered_map>
#include <string>
#include <stack>
#include <cstdlib>
#include <random>

#include "Tokens.h"
#include "Exceptions.h"
#define MARGIN_OF_ERROR 0.00001

using namespace std;
namespace ElaroDARFormula
{
     class /*__declspec(dllexport)*/ darformula
    {
         public : class FormulaElement
        {
             protected : darformula *_belongsTo;
             public : virtual const string toString() = 0;
            virtual double calcValue() = 0;
            virtual ~FormulaElement() = 0;
        };
        class SimpleElement : public FormulaElement
        {
             string _var;
             double _val;

          public:
             SimpleElement(string var, darformula *belonging);
            SimpleElement(double val, darformula *belonging);
            ~SimpleElement();
            const string toString();
            bool isVariable();
            double calcValue();
        };
        class UnaryElement : public FormulaElement
        {
             string _operator;
             FormulaElement *_operand;

          public:
             UnaryElement(string op, FormulaElement *operand, darformula *belonging);
            ~UnaryElement();
            const string toString();
            double calcValue();
        };
        class BinaryElement : public FormulaElement
        {
             string _operator;
             FormulaElement *_op1, *_op2;
             public : BinaryElement(string op, FormulaElement *first, FormulaElement *second, darformula *belonging);
            ~BinaryElement();
            const string toString();
            double calcValue();
            
        };
        class TernaryElement : public FormulaElement
        {
             string _operator;
             SimpleElement *_counter;
             FormulaElement *_limit, *_expression;
             public : TernaryElement(string op, SimpleElement *counter, FormulaElement *limit, FormulaElement *expression, darformula *belonging);
            			 ~TernaryElement();
            const string toString();
            double calcValue();
            
        };

      private:
         unordered_map<string, double> _variables;
         vector<string> _acceptedVariableNames;
         darformula::FormulaElement *_formula;
         bool _willRaiseMathematicalExceptions;

         ElaroDARFormula::darformula::FormulaElement *makeFormula(ElaroDARFormula::FormulaTokens *formula);
        public :
         darformula();
        virtual ~darformula();
        
        const string toString();
        void putVar(string name, double value);
        double getVar(string name);
        double calcValue();
        void setFormula(string expression, string acceptedVars[], int numberOfVars);
        void enableMathExceptions(bool yes);
        const vector<string> getAcceptedVars();
        void setAcceptedVars(const string acceptedVars[], const int numberOfVars);
        
    };
    
}
#endif //MATHEMODEON_DARFORMULA_H