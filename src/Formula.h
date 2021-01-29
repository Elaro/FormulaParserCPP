#ifndef ElaroSolutions_DARFormula_FORMULA_H
#define ElaroSolutions_DARFormula_FORMULA_H

#include "Nodes.h"
#include "Parser.h"
#include "Scanner.h"
#include "Exceptions.h"

#include <typeinfo>
#include <set>

namespace ElaroSolutions { namespace DARFormula {

    class Formula
    {
        std::unordered_map<std::wstring,double> _variables;
        std::set<std::wstring> _allowedVariables;
        std::set<std::wstring> _allowedFields;
        IDataStructure *_data;
        Scanner *_scanner;
        Parser *_parser;
        Node *_root;
        bool throwsExceptionFromCalcValue;
        void checkVariablesAndFields();
        void checkNode(Node *n);

        public:
        Formula();
        explicit Formula(IDataStructure *data);
        Formula(IDataStructure *data, std::wstring permittedVariables[]);
        Formula(IDataStructure *data, std::wstring permittedVariables[], std::wstring permittedFields[]);
        std::wstring setFormula(const std::wstring& formula);
        void addVariable(const std::wstring& variableName, double initialValue);
        double getVariableValue(const std::wstring& variableName);
        void updateVariable(const std::wstring& variableName, double value);
        void addOneToVariable(const std::wstring& variableName);
        void setData(IDataStructure *data);
        void addField(std::wstring fieldName);
        double calculateValue();
        void enableExceptionsOnCalculateValue();
        void disableExceptionsOnCalculateValue();
        bool exceptionsOnCalculateValueEnabled() const;
        std::wstring formulaToText();
    };
} }

#endif
