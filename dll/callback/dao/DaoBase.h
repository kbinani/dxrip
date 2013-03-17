#ifndef DXRIP_CALLBACK_DAOBASE_H_
#define DXRIP_CALLBACK_DAOBASE_H_

#include <string>
#include <vector>
#include <map>
namespace sqlite {
    #include <sqlite3.h>
}
#include "EntityBase.h"

namespace dxrip { namespace callback {

    class DaoBase {
    protected:
        sqlite::sqlite3 *connection;

    public:
        explicit DaoBase(const std::string &dbFilePath);

        ~DaoBase();

    protected:
        bool InternalQuery(const std::string &sql, std::vector<EntityBase> &result = std::vector<EntityBase>());

    private:
        static int Callback(void *arg, int argc, char **argv, char **columnNameList);
    };

} }

#endif
