#pragma once

#include "Mesh.h"

namespace com { namespace github { namespace kbinani {

    Mesh::Mesh() {
        Init();
    }

    void Mesh::Init() {
        index.clear();
        vertexList.clear();
    }

    int Mesh::FromIndexedPrimitive(
            Mesh &result,
            LPDIRECT3DDEVICE9 device,
            D3DPRIMITIVETYPE primitiveType,
            INT baseVertexIndex,
            UINT startIndex,
            UINT primitiveCount)
    {
        result.Init();

        if (primitiveType != D3DPT_TRIANGLELIST) {
            return 1;
        }

        DWORD vertexFormat;
        device->GetFVF(&vertexFormat);

        // index buffer
        D3D9Base::IDirect3DIndexBuffer9 *indexBuffer = NULL;
        if (device->GetIndices(&indexBuffer) != D3D_OK) {
            return 1;
        }

        D3D9Base::D3DINDEXBUFFER_DESC indexBufferDesciption;
        if (indexBuffer->GetDesc(&indexBufferDesciption) != D3D_OK) {
            indexBuffer->Release();
            return 1;
        }
        if (D3DFMT_INDEX16 != indexBufferDesciption.Format) {
            indexBuffer->Release();
            return 1;
        }

        uint16_t *indexBufferData = NULL;
        if (indexBuffer->Lock(0, 0, (void **)&indexBufferData, D3DLOCK_READONLY) != D3D_OK) {
            indexBuffer->Release();
            return 1;
        }

        // vertex buffer
        D3D9Base::IDirect3DVertexBuffer9 *vertexBuffer = NULL;
        UINT offsetInBytes, stride;
        if (device->GetStreamSource(0, &vertexBuffer, &offsetInBytes, &stride) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();
            return 1;
        }

        D3D9Base::D3DVERTEXBUFFER_DESC vertexBufferDescription;
        if (vertexBuffer->GetDesc(&vertexBufferDescription) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();
            vertexBuffer->Release();
            return 1;
        }
        
        void *vertexBufferData = NULL;
        if (vertexBuffer->Lock(offsetInBytes, vertexBufferDescription.Size, &vertexBufferData, D3DLOCK_READONLY) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();
            vertexBuffer->Release();
            return 1;
        }

        std::vector<uint16_t> indexMap;
        for (UINT i = 0; i < primitiveCount * 3; ++i) {
            uint16_t a = indexBufferData[baseVertexIndex + startIndex + i];
            std::vector<uint16_t>::iterator index = std::find(indexMap.begin(), indexMap.end(), a);
            if (index == indexMap.end()) {
                indexMap.push_back(a);
                void *vertexInfo = (void *)((uint8_t *)vertexBufferData + stride * a);
                float x = ((float *)vertexInfo)[0];
                float y = ((float *)vertexInfo)[1];
                float z = ((float *)vertexInfo)[2];
                result.vertexList.push_back(Vec3f(x, y, z));
                result.index.push_back((uint16_t)(indexMap.size() - 1));
            } else {
                result.index.push_back((uint16_t)std::distance(indexMap.begin(), index));
            }
        }

        indexBuffer->Unlock();
        indexBuffer->Release();
        vertexBuffer->Unlock();
        vertexBuffer->Release();

        return OK;
    }

    void Mesh::WriteFrame(std::ostream &stream, const std::string &frameName) {
        stream << "Frame " << frameName << " {" << std::endl;
        stream << "  Mesh {" << std::endl;
        stream << "    " << vertexList.size() << ";" << std::endl;
        stream.setf(std::ios_base::fixed, std::ios_base::floatfield);
        stream << std::setprecision(8);

        for (int i = 0; i < vertexList.size(); ++i) {
            Vec3f vertex = vertexList[i];
            stream << "    " << vertex.x << ";" << vertex.y << ";" << vertex.z << ";" << (i == vertexList.size() - 1 ? ";" : ",") << std::endl;
        }

        stream << std::endl;

        int primitiveCount = index.size() / 3;
        stream << "    " << primitiveCount << ";" << std::endl;
        int indexCount = 0;
        for (int i = 0; i < primitiveCount; ++i) {
            stream << "    3;";
            for (int j = 0; j < 3; ++j) {
                uint16_t a = index[indexCount + j];
                stream << a << (j < 2 ? "," : ";");
            }
            stream << (i < primitiveCount - 1 ? "," : ";") << std::endl;
            indexCount += 3;
        }

        stream << "  }" << std::endl;
        stream << "}" << std::endl;
    }

} } }