#ifndef DXRIP_CALLBACK_FILEUTIL_H_
#define DXRIP_CALLBACK_FILEUTIL_H_

#include <string>
#include <windows.h>

namespace dxrip { namespace callback {

    class FileUtil {
    public:
        /**
         * Delete directory recursively.
         * @param  directory  A directory path to be deleted.
         * @return  true if success.
         */
        bool DeleteDirectoryTree(const std::string &directory) const;

        /**
         * Create directory.
         * @param  directory  A directory path to be created.
         * @return  true if success.
         */
        bool CreateDirectory(const std::string &directory) const;

    private:
        inline bool IsDotDirectory(const WIN32_FIND_DATAA &findData) const;
    };

} }

#endif
