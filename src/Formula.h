#ifndef ElaroSolutions_DARFormula_FORMULA_H
#define ElaroSolutions_DARFormula_FORMULA_H

#include "Nodes.h"
#include "../CocoR/Parser.h.old"
#include "../CocoR/Scanner.h.old"
#include "Exceptions.h"

#include <typeinfo>
#include <set>
#include <locale>
#include <codecvt>

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
        void checkVariablesAndFields() noexcept(false);
        void checkNode(Node *n) noexcept(false);

        public:
        Formula();
        explicit Formula(std::wstring permittedVariables[]);
        Formula(std::wstring permittedVariables[], std::wstring permittedFields[]);
        void setFormula(const std::wstring& formula) noexcept(false);
        void setFormula(const std::string& formula);
        void addVariable(const std::wstring& variableName, double initialValue);
        double getVariableValue(const std::wstring& variableName) noexcept(false);
        void updateVariable(const std::wstring& variableName, double value);
        void addOneToVariable(const std::wstring& variableName);
        void setData(IDataStructure *data);
        void addField(const std::wstring& fieldName);
        double calculateValue();
        void enableExceptionsOnCalculateValue();
        void disableExceptionsOnCalculateValue();
        bool exceptionsOnCalculateValueEnabled() const;
        std::wstring formulaToText();
        ~Formula();
    };
} }

#endif
