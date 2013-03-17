#ifndef DXRIP_CALLBACK_ENTITYBASE_H_
#define DXRIP_CALLBACK_ENTITYBASE_H_

#include <map>
#include <string>

namespace dxrip { namespace callback {

    class EntityBase {
    private:
        std::map<std::string, std::string> columnValueList;

    public:
        explicit EntityBase(int argc, char **columnList, char **valueList);

        typedef std::map<std::string, std::string>::const_iterator ConstIterator;

    public:
        ConstIterator Begin() const;

        ConstIterator End() const;
    };

} }

#endif
