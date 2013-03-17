#include "EntityBase.h"

namespace dxrip { namespace callback {

    EntityBase::EntityBase(int argc, char **columnList, char **valueList) {
        columnValueList.clear();
        for (int i = 0; i < argc; ++i) {
            std::string column(columnList[i]);
            std::string value = valueList[i] ? std::string(valueList[i]) : "NULL";
            columnValueList.insert(std::make_pair(column, value));
        }
    }

    EntityBase::ConstIterator EntityBase::Begin() const {
        return columnValueList.begin();
    }

    EntityBase::ConstIterator EntityBase::End() const {
        return columnValueList.end();
    }

} }
