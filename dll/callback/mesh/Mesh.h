#pragma once

#include "../Main.h"
#include "MeshDescriptor.h"

namespace dxrip { namespace callback {

    class MeshDescriptor;

    class Mesh {
    public:
        static const int OK = 0;

        explicit Mesh(const dxrip::callback::MeshDescriptor &descriptor);
        /**
         * Write frame data to stream, as X file format.
         */
        void WriteFrame(std::ostream &stream, const std::string &textureFileName, const std::string &frameName = "Frame");

        bool IsValid() const;

        std::string GetLastError() const;

    private:
        std::vector<Vec3f> vertexList;
        std::vector<Vec2f> textureCoordList;
        std::vector<uint16_t> index;
        bool isValid;
        std::string lastError;
    };

} }
