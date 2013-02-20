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
D3D9CALLBACK_API bool ReportDrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) {
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
D3D9CALLBACK_API void ReportBeginScene() {}
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
