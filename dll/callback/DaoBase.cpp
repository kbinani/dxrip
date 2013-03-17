#include "DaoBase.h"

namespace dxrip { namespace callback {

    DaoBase::DaoBase(const std::string &dbFilePath) : connection(NULL) {
        if (sqlite::sqlite3_open(dbFilePath.c_str(), &connection)) {
            connection = NULL;
        }
    }

    DaoBase::~DaoBase() {
        if (connection) {
            sqlite::sqlite3_close(connection);
        }
    }

    bool DaoBase::InternalQuery(const std::string &sql, std::vector<EntityBase> &result) {
        result.clear();
        if (!connection) return false;
        char *errorMessage;
        if (sqlite::sqlite3_exec(connection, sql.c_str(), Callback, &result, &errorMessage) != SQLITE_OK) {
            sqlite::sqlite3_free(errorMessage);
            return false;
        }
        return true;
    }

    int DaoBase::Callback(void *arg, int argc, char **argv, char **columnNameList) {
        std::vector<EntityBase> *result = (std::vector<EntityBase> *)arg;
        result->push_back(EntityBase(argc, columnNameList, argv));
        return 0;
    }

} }
