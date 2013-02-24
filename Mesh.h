#pragma once

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class Mesh {
    public:
        static const int OK = 0;

        static int FromIndexedPrimitive(
            Mesh &result,
            LPDIRECT3DDEVICE9 device,
            D3DPRIMITIVETYPE primitiveType,
            INT BaseVertexIndex,
            UINT startIndex,
            UINT primitiveCount);

        Mesh();
        /**
         * Write frame data to stream, as X file format.
         */
        void WriteFrame(std::ostream &stream, const std::string &frameName = "Frame");

    private:
        void Init();

    private:
        std::vector<Vec3f> vertexList;
        std::vector<uint16_t> index;
    };

} } }