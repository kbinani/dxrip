#include "Main.h"
#include "d3d9Callback.h"
#include "Context.h"

using namespace com::github::kbinani;

D3D9CALLBACK_API void D3D9CallbackInitialize() {}
D3D9CALLBACK_API void D3D9CallbackFreeMemory() {}

//
// Creation callbacks
//
D3D9CALLBACK_API void ReportCreateVertexShader(CONST DWORD* pFunction, HANDLE Shader) {}
D3D9CALLBACK_API void ReportCreatePixelShader(CONST DWORD* pFunction, HANDLE Shader) {}

//
// Allocation update callbacks
//
D3D9CALLBACK_API bool ReportUnlockTexture(D3DSURFACE_DESC &Desc, Bitmap &Bmp, HANDLE Handle) { return true; }
D3D9CALLBACK_API void ReportLockVertexBuffer(BufferLockData &Data, D3DVERTEXBUFFER_DESC &Desc) {}
D3D9CALLBACK_API void ReportLockIndexBuffer(BufferLockData &Data, D3DINDEXBUFFER_DESC &Desc) {}

D3D9CALLBACK_API void ReportDestroy(HANDLE Handle) {}

//
// D3D device state update callbacks
//
D3D9CALLBACK_API void ReportSetTexture(DWORD Stage, HANDLE *SurfaceHandles, UINT SurfaceHandleCount) {}

D3D9CALLBACK_API void ReportSetViewport(CONST D3DVIEWPORT9 *pViewport) {}
D3D9CALLBACK_API void ReportSetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) {}

D3D9CALLBACK_API void ReportSetVertexDeclaration(D3DVERTEXELEMENT9 *Elements, UINT ElementCount) {}
D3D9CALLBACK_API void ReportSetFVF(DWORD FVF) {}
D3D9CALLBACK_API void ReportSetStreamSource(UINT StreamNumber, HANDLE VBufferHandle, UINT OffsetInBytes, UINT Stride) {}

D3D9CALLBACK_API void ReportSetLight(DWORD Index, CONST D3DLIGHT9* pLight) {}
D3D9CALLBACK_API void ReportLightEnable(DWORD Index, BOOL Enable) {}

D3D9CALLBACK_API void ReportSetMaterial(CONST D3DMATERIAL9* pMaterial) {}

D3D9CALLBACK_API void ReportSetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {}
D3D9CALLBACK_API void ReportSetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {}

D3D9CALLBACK_API void ReportSetIndices(HANDLE IBufferHandle) {}

D3D9CALLBACK_API void ReportSetVertexShader(HANDLE Shader) {}
D3D9CALLBACK_API void ReportSetPixelShader(HANDLE Shader) {}

D3D9CALLBACK_API void ReportSetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) {}
D3D9CALLBACK_API void ReportSetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) {}
D3D9CALLBACK_API void ReportSetVertexShaderConstantI(UINT StartRegister, CONST UINT* pConstantData, UINT Vector4iCount) {}

D3D9CALLBACK_API void ReportSetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) {}
D3D9CALLBACK_API void ReportSetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) {}
D3D9CALLBACK_API void ReportSetPixelShaderConstantI(UINT StartRegister, CONST UINT* pConstantData, UINT Vector4iCount) {}

D3D9CALLBACK_API void ReportSetRenderTarget(DWORD RenderTargetIndex, HANDLE Surface) {}

//
// Render callbacks
//
D3D9CALLBACK_API bool ReportDrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) {
    return true;
}
D3D9CALLBACK_API bool ReportDrawIndexedPrimitive(
    D3DPRIMITIVETYPE PrimitiveType,
    INT BaseVertexIndex,
    UINT MinIndex,
    UINT NumVertices,
    UINT StartIndex,
    UINT PrimitiveCount
) {
    Context *context = Context::Instance();
    D3D9Base::IDirect3DDevice9 *device = context->device;
    ID3D9DeviceOverlay *overlay = context->screenOverlay;
    if (PrimitiveType == D3D9Base::D3DPT_TRIANGLELIST) {
        UINT start = StartIndex;
        static int count = 0;
        static int lastSceneCount = -1;

        // index buffer
        D3D9Base::IDirect3DIndexBuffer9 *indexBuffer = NULL;
        if (device->GetIndices(&indexBuffer) != D3D_OK) {
            overlay->WriteLine(String("ReportDrawIndexedPrimitive; GetIndices failed"), RGBColor(), 0);
            return true;
        }

        D3D9Base::D3DINDEXBUFFER_DESC indexBufferDesciption;
        if (indexBuffer->GetDesc(&indexBufferDesciption) != D3D_OK) {
            indexBuffer->Release();
            overlay->WriteLine(String("ReportDrawIndexedPrimitive; IDirect3DIndexBuffer9::GetDesc failed"), RGBColor(), 0);
            return true;
        }
        if (D3DFMT_INDEX16 != indexBufferDesciption.Format) {
            indexBuffer->Release();
            overlay->WriteLine(
                String("ReportDrawIndexedPrimitive; indexBufferDesciption.Format=") + 
                    String(indexBufferDesciption.Format) + 
                    String(", which is not supported"),
                RGBColor(),
                0
            );
            return true;
        }

        uint16_t *indexBufferData = NULL;
        if (indexBuffer->Lock(0, 0, (void **)&indexBufferData, D3DLOCK_READONLY) != D3D_OK) {
            indexBuffer->Release();
            overlay->WriteLine(String("ReportDrawIndexedPrimitive; IDirect3DIndexBuffer9::Lock failed"), RGBColor(), 0);
            return true;
        }

        // vertex buffer
        D3D9Base::IDirect3DVertexBuffer9 *vertexBuffer = NULL;
        UINT offsetInBytes, stride;
        if (device->GetStreamSource(0, &vertexBuffer, &offsetInBytes, &stride) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();
            overlay->WriteLine(String("ReportDrawIndexedPrimitive; GetStreamSource failed"), RGBColor(), 0);
            return true;
        }

        D3D9Base::D3DVERTEXBUFFER_DESC vertexBufferDescription;
        if (vertexBuffer->GetDesc(&vertexBufferDescription) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();

            vertexBuffer->Release();
            overlay->WriteLine(String("ReportDrawIndexedPrimitive; IDirect3DVertexBuffer9::GetDesc failed"), RGBColor(), 0);
            return true;
        }

        void *vertexBufferData = NULL;
        if (vertexBuffer->Lock(offsetInBytes, vertexBufferDescription.Size, &vertexBufferData, D3DLOCK_READONLY) != D3D_OK) {
            indexBuffer->Unlock();
            indexBuffer->Release();

            vertexBuffer->Release();
            overlay->WriteLine(String("ReportDrawIndexedPrimitive; IDirect3DVertexBuffer9::Lock failed"), RGBColor(), 0);
            return true;
        }

        std::map<uint16_t, UINT> indexMap;
        std::vector<void *> vertexList;
        for (UINT i = 0; i < PrimitiveCount * 3; ++i) {
            uint16_t a = indexBufferData[BaseVertexIndex + MinIndex + i];
            overlay->WriteLine(String("ReportDrawIndexedPrimitive; a=") + String(a) + String("; vertexBufferDescription.Size=") + String(vertexBufferDescription.Size), RGBColor(), 0);
            if (indexMap.find(a) == indexMap.end()) {
                indexMap.insert(std::make_pair(a, indexMap.size()));
                vertexList.push_back((void *)((uint8_t *)vertexBufferData + stride * a));
            }
        }

        std::ostringstream stream;
        if (lastSceneCount != context->sceneCount) {
            count = 0;
        }
        lastSceneCount = context->sceneCount;
        stream << "C:\\ProgramData\\Temp\\d3d9callback\\" << context->sceneCount << "_" << count << ".x";
        std::string filePath = stream.str();

        std::ofstream file;
        file.open(filePath);
        file << "xof 0302txt 0064" << std::endl;
        file << "Mesh {" << std::endl;
        file << "  " << vertexList.size() << ";" << std::endl;
        for (int i = 0; i < vertexList.size(); ++i) {
            void *vertexInfo = vertexList[i];
            float x = ((float *)vertexInfo)[0];
            float y = ((float *)vertexInfo)[1];
            float z = ((float *)vertexInfo)[2];
            file << "  " << x << ";" << y << ";" << z << ";" << (i == vertexList.size() - 1 ? ";" : ",") << std::endl;
        }

        file << std::endl;

        file << "  " << PrimitiveCount << ";" << std::endl;
        int index = 0;
        for (int i = 0; i < PrimitiveCount; ++i) {
            file << "  3;";
            for (int j = 0; j < 3; ++j) {
                uint16_t a = indexBufferData[BaseVertexIndex + MinIndex + index + j];
                UINT actualIndex = indexMap[a];
                file << actualIndex << (j < 2 ? "," : ";");
            }
            file << (i < PrimitiveCount - 1 ? "," : ";") << std::endl;
            index += 3;
        }

        file << "}" << std::endl;
        file.close();

        indexBuffer->Unlock();
        indexBuffer->Release();
        vertexBuffer->Unlock();
        vertexBuffer->Release();

        ++count;
    }
    return true;
}
D3D9CALLBACK_API bool ReportDrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    return true;
}
D3D9CALLBACK_API bool ReportDrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    return true;
}

//
// Frame buffer update callbacks
//
D3D9CALLBACK_API void ReportPresent(CONST RECT* pSourceRect,CONST RECT* pDestRect) {}
D3D9CALLBACK_API void ReportClear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) {}

//
// Scene updates
//
D3D9CALLBACK_API void ReportBeginScene() {
    Context *context = Context::Instance();
    context->sceneCount++;
}

D3D9CALLBACK_API void ReportEndScene() {}

//
// Device reference updates
//
D3D9CALLBACK_API void ReportCreateDevice(D3D9Base::LPDIRECT3DDEVICE9 Device, ID3D9DeviceOverlay *Overlay) {
    Context *context = Context::Instance();
    context->screenOverlay = Overlay;
    context->device = Device;
}
D3D9CALLBACK_API void ReportFreeDevice() {}

D3D9CALLBACK_API bool PreRenderQuery(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount) {
    return false;
}
D3D9CALLBACK_API void ReportSetStreamSourceFreq(UINT StreamNumber, UINT FrequencyParameter) {}
