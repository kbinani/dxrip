#include "Config.h"

namespace dxrip { namespace callback {

    Config::Config(const std::string &directory) {
        this->directory = directory;
    }

    const std::string Config::GetSceneDataDirectory() const {
        return GetRepositoryDirectory() + "\\scene";
    }

    const std::string Config::GetMeshDataDirectory() const {
        return GetRepositoryDirectory() + "\\mesh";
    }

    const std::string Config::GetRepositoryDirectory() const {
        return directory + ".dxrip";
    }

} }