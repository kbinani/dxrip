#ifndef DXRIP_CALLBACK_SCENEDAO_H_
#define DXRIP_CALLBACK_SCENEDAO_H_

#include "Main.h"
#include "DaoBase.h"
#include "SceneObject.h"

namespace dxrip { namespace callback {

    class SceneObjectDao : public DaoBase {
    private:
        sqlite::sqlite3 *connection;

    public:
        explicit SceneObjectDao(const std::string &dbFilePath);

        void Add(int sceneNumber, const std::string &objectId);

    private:
        void InitializeTable();
    };

} }

#endif
