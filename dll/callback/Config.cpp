#include "Config.h"

namespace com { namespace github { namespace kbinani {

    Config::Config(const std::string &directory) {
        this->directory = directory;
    }

    const std::string Config::GetFrameDataDirectory() const {
        return GetRepositoryDirectory() + "\\frame";
    }

    const std::string Config::GetMeshDataDirectory() const {
        return GetRepositoryDirectory() + "\\mesh";
    }

    const std::string Config::GetRepositoryDirectory() const {
        return directory + ".dxrip";
    }

} } }
