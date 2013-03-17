#include "SceneObjectDao.h"
#include <sstream>

namespace dxrip { namespace callback {

    SceneObjectDao::SceneObjectDao(const std::string &dbFilePath) : DaoBase(dbFilePath) {
        InitializeTable();
    }

    void SceneObjectDao::Add(int sceneNumber, const std::string &objectId) {
        std::ostringstream stream;
        stream << "INSERT INTO `scene_objects`(`scene`, `object_id`) VALUES(" << sceneNumber << ", '" << objectId << "');";

        InternalQuery("BEGIN IMMEDIATE TRANSACTION;");
        InternalQuery(stream.str());
        InternalQuery("COMMIT TRANSACTION;");
    }

    void SceneObjectDao::InitializeTable() {
        std::vector<EntityBase> result;
        InternalQuery("PRAGMA TABLE_INFO(scene_objects);", result);
        if (result.empty()) {
            InternalQuery("BEGIN EXCLUSIVE TRANSACTION;");
            InternalQuery("CREATE TABLE `scene_objects`(`scene` INTEGER, `object_id` TEXT NOT NULL);");
            InternalQuery("COMMIT TRANSACTION;");
        }
        InternalQuery("BEGIN EXCLUSIVE TRANSACTION;");
        InternalQuery("DELETE FROM `scene_objects`;");
        InternalQuery("COMMIT TRANSACTION;");
    }

} }
