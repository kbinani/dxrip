#include "FileUtil.h"

namespace dxrip { namespace callback {

    bool FileUtil::DeleteDirectoryTree(const std::string &directory) const {
        WIN32_FIND_DATA findData;
        HANDLE finderHandle = ::FindFirstFileA((directory + "\\*").c_str(), &findData);
        if (INVALID_HANDLE_VALUE == finderHandle) {
            return false;
        }
        do {
            if (IsDotDirectory(findData)) {
                continue;
            }
            std::string foundPath = directory + "\\" + std::string(findData.cFileName);
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (!this->DeleteDirectoryTree(foundPath)) {
                    ::FindClose(finderHandle);
                    return false;
                }
            } else {
                if (!::DeleteFileA(foundPath.c_str())) {
                    ::FindClose(finderHandle);
                    return false;
                }
            }
        } while (::FindNextFileA(finderHandle, &findData));
        ::FindClose(finderHandle);
        return ::RemoveDirectoryA(directory.c_str()) == TRUE;
    }

    bool FileUtil::CreateDirectory(const std::string &directory) const {
        return ::CreateDirectoryA(directory.c_str(), NULL) != 0;
    }

    bool FileUtil::IsDotDirectory(const WIN32_FIND_DATA &findData) const {
        return std::string(findData.cFileName) == std::string(".") ||
               std::string(findData.cFileName) == std::string("..");
    }

} }
