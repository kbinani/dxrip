#pragma once

#include "Mesh.h"
#include "MeshDescriptor.h"
#include "D3D9Util.h"

namespace com { namespace github { namespace kbinani {

    Mesh::Mesh(const MeshDescriptor &descriptor) {
        index.clear();
        vertexList.clear();
        textureCoordList.clear();
        isValid = false;
        lastError = "";

        LPDIRECT3DDEVICE9 device = descriptor.Device();
        D3DPRIMITIVETYPE primitiveType = descriptor.Type();
        UINT primitiveCount = descriptor.VertexIndexCount() / 3;
        UINT vertexIndexOffset = descriptor.VertexIndexOffset();

        if (primitiveType != D3DPT_TRIANGLELIST) {
            lastError = "primitive type is not D3DPT_TRIANGLELIST";
            return;
        }

        DWORD vertexFormat;
        device->GetFVF(&vertexFormat);

        if ((vertexFormat & D3DFVF_TEX0) != D3DFVF_TEX0) {
            lastError = "vertex format doesn't contains D3DFVF_TEX0";
            return;
        }
        if ((vertexFormat & D3DFVF_TEX1) != D3DFVF_TEX1) {
            lastError = "vertex format doesn't contains D3DFVF_TEX1";
            return;
        }

        // index buffer
        D3D9Base::IDirect3DIndexBuffer9 *indexBuffer = NULL;
        if (device->GetIndices(&indexBuffer) != D3D_OK) {
            lastError = "IDirect3DDevice9::GetIndices failed";
            return;
        }

        D3D9Base::D3DINDEXBUFFER_DESC indexBufferDesciption;
        if (indexBuffer->GetDesc(&indexBufferDesciption) != D3D_OK) {
            indexBuffer->Release();
            lastError = "IDirect3DIndexBuffer9::GetDesc failed";
            return;
        }
        if (D3DFMT_INDEX16 != indexBufferDesciption.Format) {
            indexBuffer->Release();
            lastError = "index format is not D3DFMT_INDEX16";
            return;
        }

        uint16_t *indexBufferData = NULL;
        if (indexBuffer->Lock(0, 0, (void **)&indexBufferData, D3DLOCK_READONLY) != D3D_OK) {
            indexBuffer->Release();
            lastError = "IDirect3DIndexBuffer9::Lock failed";
            return;
        }

        // vertex buffer
        D3D9Base::IDirect3DVertexBuffer9 *vertexBuffer = NULL;
        UINT offsetInBytes, stride;
        if (device->GetStreamSource(0, &vertexBuffer, &offsetInBytes, &stride) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();
            lastError = "IDirect3DDevice9::GetStreamSource failed";
            return;
        }

        D3D9Base::D3DVERTEXBUFFER_DESC vertexBufferDescription;
        if (vertexBuffer->GetDesc(&vertexBufferDescription) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();
            vertexBuffer->Release();
            lastError = "IDirect3DVertexBuffer9::GetDesc failed";
            return;
        }
        
        void *vertexBufferData = NULL;
        if (vertexBuffer->Lock(offsetInBytes, vertexBufferDescription.Size, &vertexBufferData, D3DLOCK_READONLY) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();
            vertexBuffer->Release();
            lastError = "IDirect3DVertexBuffer9::Lock failed";
            return;
        }

        std::vector<uint16_t> indexMap;
        for (UINT i = 0; i < primitiveCount * 3; ++i) {
            uint16_t a = indexBufferData[vertexIndexOffset + i];
            std::vector<uint16_t>::iterator it = std::find(indexMap.begin(), indexMap.end(), a);
            if (it == indexMap.end()) {
                indexMap.push_back(a);
                void *vertexInfo = (void *)((uint8_t *)vertexBufferData + stride * a);
                float x = ((float *)vertexInfo)[0];
                float y = ((float *)vertexInfo)[1];
                float z = ((float *)vertexInfo)[2];
                void *uv = (void *)((uint8_t *)vertexInfo + stride - 2 * sizeof(float));
                float u = ((float *)uv)[0];
                float v = ((float *)uv)[1];
                vertexList.push_back(Vec3f(x, y, z));
                textureCoordList.push_back(Vec2f(u, v));
                index.push_back((uint16_t)(indexMap.size() - 1));
            } else {
                index.push_back((uint16_t)std::distance(indexMap.begin(), it));
            }
        }

        indexBuffer->Unlock();
        indexBuffer->Release();
        vertexBuffer->Unlock();
        vertexBuffer->Release();

        isValid = true;
    }

    void Mesh::WriteFrame(std::ostream &stream, const std::string &textureFileName, const std::string &frameName) {
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

        stream << "    MeshTextureCoords {" << std::endl;
        stream << "      " << textureCoordList.size() << ";" << std::endl;
        for (int i = 0; i < textureCoordList.size(); ++i) {
            Vec2f uv = textureCoordList[i];
            stream << "      " << uv.x << ";" << uv.y << ";" << (i == textureCoordList.size() - 1 ? ";" : ",") << std::endl;
        }
        stream << "    }" << std::endl;

        stream << "    MeshMaterialList {" << std::endl;
        stream << "      1;" << std::endl;
        stream << "      " << primitiveCount << ";" << std::endl;
        for (int i = 0; i < primitiveCount; ++i) {
            stream << "      0" << (i == primitiveCount - 1 ? ";" : ",") << std::endl;
        }
        stream << "      Material {" << std::endl;
        stream << "        1.0;1.0;1.0;0.0;;" << std::endl;
        stream << "        0.000000;" << std::endl;
        stream << "        0.000000;0.000000;0.000000;;" << std::endl;
        stream << "        0.000000;0.000000;0.000000;;" << std::endl;
        stream << "        TextureFilename {" << std::endl;
        stream << "          \"" << textureFileName << "\";" << std::endl;
        stream << "        }" << std::endl;
        stream << "      }" << std::endl;
        stream << "    }" << std::endl;

        stream << "  }" << std::endl;
        stream << "}" << std::endl;
    }

    bool Mesh::IsValid() const {
        return isValid;
    }

    std::string Mesh::GetLastError() const {
        return lastError;
    }

} } }