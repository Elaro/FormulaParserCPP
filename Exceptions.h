#ifndef ElaroSolutions_DARFormula_EXCEPTIONS_H
#define ElaroSolutions_DARFormula_EXCEPTIONS_H

#include <stdexcept>

namespace ElaroSolutions::DARFormula{
    class UnexpectedVariable : std::runtime_error {
        public:
        UnexpectedVariable(std::string what_arg) : runtime_error(what_arg.c_str()){}
    };

    class UninitializedVariable : std::out_of_range {
        public:
        UninitializedVariable(std::string what_arg) : out_of_range(what_arg.c_str()){}
    };
}

#endif