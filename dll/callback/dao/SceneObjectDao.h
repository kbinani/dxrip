#ifndef DXRIP_CALLBACK_SCENEDAO_H_
#define DXRIP_CALLBACK_SCENEDAO_H_

#include "DaoBase.h"
#include <boost/thread.hpp>

namespace dxrip { namespace callback {

    class SceneObjectDao : public DaoBase {
    private:
        static const int MAX_BUFFER_COUNT = 10000;

        sqlite::sqlite3 *connection;
        std::vector<std::string> bufferedQuery;
        boost::thread *bufferDispatchThread;

    public:
        explicit SceneObjectDao(const std::string &dbFilePath);

        ~SceneObjectDao();

        void AddAsync(int sceneNumber, const std::string &objectId);

        void Add(int sceneNumber, const std::string &objectId);

    private:
        void InitializeTable();

        void DispatchBufferedQuery(std::vector<std::string> &bufferedQuery);
    };

} }

#endif
