#ifndef ElaroSolutions_DARFormula_FORMULA_H
#define ElaroSolutions_DARFormula_FORMULA_H

#include "Nodes.h"
#include "Parser.h"
#include "Scanner.h"
#include "Exceptions.h"

#include <typeinfo>

namespace ElaroSolutions::DARFormula {

    class Formula
    {
        std::unordered_map<std::string,double> _variables;
        std::vector<std::string> _allowedVariables;
        std::vector<std::string> _allowedFields;
        IDataStructure *_data;
        Scanner *_scanner;
        Parser *_parser;
        Node *_root;
        bool throwsExceptionFromCalcValue;
        void checkVariablesAndFields();
        void checkNode(Node *n);

        public:
        Formula();
        Formula(IDataStructure *data);
        Formula(IDataStructure *data, std::string formula);
        Formula(IDataStructure *data, std::string formula, std::string permittedVariables[]);
        void setFormula(std::string formula);
        void addVariable(std::string variableName, double initialValue);
        double getVariableValue(std::string variableName);
        void updateVariable(std::string variableName, double value);
        void addOneToVariable(std::string variableName);
        void setData(IDataStructure *data);
        void addAllowedField(std::string fieldName);
        double calculateValue();
        void enableExceptionsOnCalculateValue();
        void disableExceptionsOnCalculateValue();
        bool exceptionsOnCalculateValueEnabled();
        std::string formulaToString();
    };
}

#endif