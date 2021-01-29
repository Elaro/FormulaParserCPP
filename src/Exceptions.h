#ifndef ElaroSolutions_DARFormula_EXCEPTIONS_H
#define ElaroSolutions_DARFormula_EXCEPTIONS_H

#include <stdexcept>

namespace ElaroSolutions { namespace DARFormula{
    class UnexpectedVariable : std::runtime_error {
        public:
        explicit UnexpectedVariable(const std::wstring& what_arg) : runtime_error(
                reinterpret_cast<const char *>(what_arg.c_str())){}
    };

    class BadFormula: std::runtime_error {
        public:
        explicit BadFormula(const std::wstring& what_arg) : runtime_error(
                reinterpret_cast<const char *>(what_arg.c_str())){}
    };

    class UninitializedVariable : std::out_of_range {
        public:
        explicit UninitializedVariable(const std::wstring& what_arg) : out_of_range(
                reinterpret_cast<const char *>(what_arg.c_str())){}
    };
} }

#endif
