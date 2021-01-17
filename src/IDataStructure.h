#ifndef ElaroSolutions_DARFormula_IDATASTRUCTURE_H
#define ElaroSolutions_DARFormula_IDATASTRUCTURE_H

#include <string>

namespace ElaroSolutions::DARFormula
{
    class IDataStructure
    {
        public:
        virtual double getValueAt(int indexes[], std::string field)=0;
    };
} // namespace ElaroSolutions::DARFormula

#endif