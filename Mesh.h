#pragma once

#include "Main.h"
#include "MeshDescriptor.h"

namespace com { namespace github { namespace kbinani {

    class Mesh {
    public:
        static const int OK = 0;

        explicit Mesh(const MeshDescriptor &descriptor);
        /**
         * Write frame data to stream, as X file format.
         */
        void WriteFrame(std::ostream &stream, const std::string &frameName = "Frame");

        bool IsValid() const;

        std::string GetLastError() const;

    private:
        std::vector<Vec3f> vertexList;
        std::vector<uint16_t> index;
        bool isValid;
        std::string lastError;
    };

} } }