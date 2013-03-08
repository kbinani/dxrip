#pragma once

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class MeshDescriptor {
    public:
        MeshDescriptor(IDirect3DDevice9 *device, D3DPRIMITIVETYPE type, UINT vertexIndexOffset, UINT vertexIndexCount) {
            this->device = device;
            this->type = type;
            this->vertexIndexOffset = vertexIndexOffset;
            this->vertexIndexCount = vertexIndexCount;
            CalculateHash();
        }

        std::string Hash() const {
            return hash;
        }

        void CalculateHash() {
            int devicePointer = (int)this->device;
            clx::sha1 encoder;
            encoder.reset();
            encoder.update((const char *)&devicePointer, sizeof(int));
            encoder.update((const char *)&type, sizeof(D3DPRIMITIVETYPE));
            encoder.update((const char *)&vertexIndexOffset, sizeof(UINT));
            encoder.update((const char *)&vertexIndexCount, sizeof(UINT));
            encoder.finish();
            hash = encoder.to_string();
        }

        IDirect3DDevice9 *Device() const {
            return device;
        }

        D3DPRIMITIVETYPE Type() const {
            return type;
        }

        UINT VertexIndexOffset() const {
            return vertexIndexOffset;
        }

        UINT VertexIndexCount() const {
            return vertexIndexCount;
        }

    private:
        IDirect3DDevice9 *device;
        D3DPRIMITIVETYPE type;
        UINT vertexIndexOffset;
        UINT vertexIndexCount;
        std::string hash;
    };

} } }