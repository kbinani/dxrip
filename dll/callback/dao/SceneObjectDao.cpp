#include "SceneObjectDao.h"
#include <sstream>

namespace dxrip { namespace callback {

    SceneObjectDao::SceneObjectDao(const std::string &dbFilePath)
        : DaoBase(dbFilePath), bufferDispatchThread(0) {
        InitializeTable();
    }

    SceneObjectDao::~SceneObjectDao() {
        DispatchBufferedQuery(bufferedQuery);
    }

    void SceneObjectDao::Add(int sceneNumber, const std::string &objectId) {
        std::ostringstream stream;
        stream << "INSERT INTO `scene_objects`(`scene`, `object_id`) VALUES(" << sceneNumber << ", '" << objectId << "');";

        InternalQuery("BEGIN IMMEDIATE TRANSACTION;");
        InternalQuery(stream.str());
        InternalQuery("COMMIT TRANSACTION;");
    }

    void SceneObjectDao::AddAsync(int sceneNumber, const std::string &objectId) {
        std::ostringstream stream;
        stream << "INSERT INTO `scene_objects`(`scene`, `object_id`) VALUES(" << sceneNumber << ", '" << objectId << "');";
        bufferedQuery.push_back(stream.str());

        if (MAX_BUFFER_COUNT <= bufferedQuery.size()) {
            if (bufferDispatchThread && bufferDispatchThread->joinable()) {
                bufferDispatchThread->join();
            }
            delete bufferDispatchThread;
            std::vector<std::string> copiedBuffer;
            copiedBuffer.reserve(bufferedQuery.size());
            std::copy(bufferedQuery.begin(), bufferedQuery.end(), std::back_inserter(copiedBuffer));
            bufferedQuery.clear();
            bufferDispatchThread = new boost::thread(boost::bind(&SceneObjectDao::DispatchBufferedQuery, this, copiedBuffer));
        }
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
        InternalQuery("VACUUM;");
        InternalQuery("COMMIT TRANSACTION;");
    }

    void SceneObjectDao::DispatchBufferedQuery(std::vector<std::string> &bufferedQuery) {
        if (!bufferedQuery.empty()) {
            InternalQuery("BEGIN IMMEDIATE TRANSACTION;");
            for (std::vector<std::string>::iterator i = bufferedQuery.begin(); i != bufferedQuery.end(); ++i) {
                std::string query = (*i);
                InternalQuery(query);
            }
            InternalQuery("COMMIT TRANSACTION;");
            bufferedQuery.clear();
        }
    }

} }
