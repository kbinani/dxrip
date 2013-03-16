#pragma once

#include <string>

namespace dxrip { namespace callback {

    /**
     * A configuration class to manage common configuration.
     */
    class Config {
    public:
        /**
         * Initialize by directory path.
         *
         * @param  directory  A directory, where callback dll is deployed.
         */
        explicit Config(const std::string &directory);

        /**
         * Get directory path for mesh data. Typically returns ${directory}\.dxrip\mesh.
         *
         * @returns  A directory path.
         */
        const std::string GetMeshDataDirectory() const;

        /**
         * Get directory path for scene data. Typically returns ${directory}\.dxrip\scene.
         *
         * @returns  A directory path.
         */
        const std::string GetSceneDataDirectory() const;

    private:
        const std::string GetRepositoryDirectory() const;

    private:
        std::string directory;
    };

} }
