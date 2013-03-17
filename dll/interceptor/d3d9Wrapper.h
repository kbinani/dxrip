/*
D3D9Wrapper.h
Written by Matthew Fisher

Contains the function declaration for all wrapped D3D9 functions.
*/

#pragma once

//
// Forward declerations
//
class IDirect3DUnknown;
class IDirect3D9;
class IDirect3DDevice9;
class IDirect3DBaseTexture9;
class IDirect3DTexture9;
//class IDirect3DCubeTexture9;
//class IDirect3DVolumeTexture9;
class IDirect3DSurface9;
//class IDirect3DVolume9;
class IDirect3DSwapChain9;
class IDirect3DVertexBuffer9;
class IDirect3DIndexBuffer9;

#define IDirect3DStateBlock9			D3D9Base::IDirect3DStateBlock9
#define IDirect3DVertexDeclaration9		D3D9Base::IDirect3DVertexDeclaration9
#define IDirect3DVertexShader9			D3D9Base::IDirect3DVertexShader9
#define IDirect3DPixelShader9			D3D9Base::IDirect3DPixelShader9
#define IDirect3DQuery9					D3D9Base::IDirect3DQuery9
#define IDirect3DCubeTexture9			D3D9Base::IDirect3DCubeTexture9
#define IDirect3DVolumeTexture9			D3D9Base::IDirect3DVolumeTexture9
#define IDirect3DVolume9				D3D9Base::IDirect3DVolume9

#define D3DADAPTER_IDENTIFIER9			D3D9Base::D3DADAPTER_IDENTIFIER9
#define D3DDEVTYPE						D3D9Base::D3DDEVTYPE
#define D3DFORMAT						D3D9Base::D3DFORMAT
#define D3DFORMAT						D3D9Base::D3DFORMAT
#define D3DDISPLAYMODE					D3D9Base::D3DDISPLAYMODE
#define D3DRESOURCETYPE					D3D9Base::D3DRESOURCETYPE
#define D3DMULTISAMPLE_TYPE				D3D9Base::D3DMULTISAMPLE_TYPE
#define D3DCAPS9						D3D9Base::D3DCAPS9
#define D3DPRESENT_PARAMETERS			D3D9Base::D3DPRESENT_PARAMETERS
#define D3DDEVICE_CREATION_PARAMETERS	D3D9Base::D3DDEVICE_CREATION_PARAMETERS
#define D3DBACKBUFFER_TYPE				D3D9Base::D3DBACKBUFFER_TYPE
#define D3DRASTER_STATUS				D3D9Base::D3DRASTER_STATUS
#define D3DGAMMARAMP					D3D9Base::D3DGAMMARAMP
#define D3DPOOL							D3D9Base::D3DPOOL
#define D3DDEVICE_CREATION_PARAMETERS	D3D9Base::D3DDEVICE_CREATION_PARAMETERS
#define D3DRENDERSTATETYPE				D3D9Base::D3DRENDERSTATETYPE
#define D3DMATERIAL9					D3D9Base::D3DMATERIAL9
#define D3DLIGHT9						D3D9Base::D3DLIGHT9
#define D3DQUERYTYPE					D3D9Base::D3DQUERYTYPE
#define D3DSTATEBLOCKTYPE				D3D9Base::D3DSTATEBLOCKTYPE
#define D3DCOLOR						D3D9Base::D3DCOLOR
#define D3DTEXTUREFILTERTYPE			D3D9Base::D3DTEXTUREFILTERTYPE
#define D3DTRANSFORMSTATETYPE			D3D9Base::D3DTRANSFORMSTATETYPE
#define D3DRECT							D3D9Base::D3DRECT
#define D3DMATRIX						D3D9Base::D3DMATRIX
#define D3DVIEWPORT9					D3D9Base::D3DVIEWPORT9
#define D3DCLIPSTATUS9					D3D9Base::D3DCLIPSTATUS9
#define D3DTEXTURESTAGESTATETYPE		D3D9Base::D3DTEXTURESTAGESTATETYPE
#define D3DSAMPLERSTATETYPE				D3D9Base::D3DSAMPLERSTATETYPE
#define D3DPRIMITIVETYPE				D3D9Base::D3DPRIMITIVETYPE
#define D3DTRIPATCH_INFO				D3D9Base::D3DTRIPATCH_INFO
#define D3DRECTPATCH_INFO				D3D9Base::D3DRECTPATCH_INFO
#define D3DVERTEXELEMENT9				D3D9Base::D3DVERTEXELEMENT9
#define D3DSURFACE_DESC					D3D9Base::D3DSURFACE_DESC
#define D3DLOCKED_RECT					D3D9Base::D3DLOCKED_RECT
#define D3DVERTEXBUFFER_DESC			D3D9Base::D3DVERTEXBUFFER_DESC
#define D3DINDEXBUFFER_DESC				D3D9Base::D3DINDEXBUFFER_DESC

#undef PURE
#define PURE

typedef D3D9Base::LPDIRECT3D9 (WINAPI *D3DCREATE)(UINT);

class IDirect3DUnknown
{
protected:
    IUnknown*   m_pUnk;
    ULONG       m_ulRef;

public:
    IDirect3DUnknown(IUnknown* pUnk)
    {
        m_pUnk = pUnk;
        m_ulRef = 1;
    }

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        return E_FAIL;
    }

    STDMETHOD_(ULONG,AddRef)(THIS)
    {
        m_pUnk->AddRef();
        return ++m_ulRef;
    }

    STDMETHOD_(ULONG,Release)(THIS)
    {
        m_pUnk->Release();

        ULONG ulRef = --m_ulRef;
        if( 0 == ulRef )
        {
            delete this;
            return 0;
        }
        return ulRef;
    }
};

class IDirect3D9 : public IDirect3DUnknown
{
protected:
    D3D9Base::LPDIRECT3D9		m_pD3D;
    static ThreadSafePointerSet	m_List;
public:
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
	{
		return E_FAIL;
	}
	IDirect3D9(D3D9Base::LPDIRECT3D9 pD3D);
    static IDirect3D9* GetDirect3D(D3D9Base::LPDIRECT3D9 pD3D);
    inline D3D9Base::LPDIRECT3D9 GetDirect3D9() { return m_pD3D; }

    /*** IDirect3DUnknown methods ***/
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3D9 methods ***/
    STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction) PURE;
    STDMETHOD_(UINT, GetAdapterCount)(THIS) PURE;
    STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) PURE;
    STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format) PURE;
    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) PURE;
    STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) PURE;
    STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) PURE;
    STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) PURE;
    STDMETHOD(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) PURE;
    STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) PURE;
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter) PURE;
    STDMETHOD(CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface) PURE;

};

class IDirect3DDevice9 : public IDirect3DUnknown
{
protected:
    D3D9Base::LPDIRECT3DDEVICE9  m_pDevice;
	D3D9Base::LPDIRECT3DSURFACE9 m_pScratchSurface;
	Overlay						 m_Overlay;
    static ThreadSafePointerSet	 m_List;
    static ThreadSafePointerSet	 m_DeclList;
    static ThreadSafePointerSet	 m_VertexShaderList;
    static ThreadSafePointerSet	 m_PixelShaderList;

public:
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
	{
		return E_FAIL;
	}
    IDirect3DDevice9(D3D9Base::LPDIRECT3DDEVICE9 pDevice);
    static IDirect3DDevice9* GetDirect3DDevice(D3D9Base::LPDIRECT3DDEVICE9 pDevice);

	void BitmapFromSurface( D3D9Base::LPDIRECT3DSURFACE9 pSurface, const RECT &Rect, Bitmap &Bmp);
	__forceinline D3D9Base::LPDIRECT3DDEVICE9 GetD3D9Device() { return m_pDevice; }
	__forceinline D3D9Base::LPDIRECT3DSURFACE9 GetScratchSurface() { return m_pScratchSurface; }
	__forceinline Overlay& GetOverlay() { return m_Overlay; }

    /*** IDirect3DUnknown methods ***/
    STDMETHOD_(ULONG,Release)(THIS);

    /*** IDirect3DDevice8 methods ***/
    STDMETHOD(TestCooperativeLevel)(THIS) PURE;
    STDMETHOD_(UINT, GetAvailableTextureMem)(THIS) PURE;
    STDMETHOD(EvictManagedResources)(THIS) PURE;
    STDMETHOD(GetDirect3D)(THIS_ IDirect3D9** ppD3D9) PURE;
    STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) PURE;
    STDMETHOD(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters) PURE;
    STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap) PURE;
    STDMETHOD_(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags) PURE;
    STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) PURE;
    STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain) PURE;
    STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain) PURE;
    STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS) PURE;
    STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) PURE;
    STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) PURE;
    STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) PURE;
    STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) PURE;
    STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) PURE;
    STDMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) PURE;
    STDMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) PURE;
    STDMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture) PURE;
    STDMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) PURE;
    STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface) PURE;
    STDMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) PURE;
    STDMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color) PURE;
    STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) PURE;
    STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget) PURE;
    STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) PURE;
    STDMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) PURE;
    STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) PURE;
    STDMETHOD(BeginScene)(THIS) PURE;
    STDMETHOD(EndScene)(THIS) PURE;
    STDMETHOD(Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) PURE;
    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix) PURE;
    STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix) PURE;
    STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE,CONST D3DMATRIX*) PURE;
    STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) PURE;
    STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) PURE;
    STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) PURE;
    STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) PURE;
    STDMETHOD(SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9*) PURE;
    STDMETHOD(GetLight)(THIS_ DWORD Index,D3DLIGHT9*) PURE;
    STDMETHOD(LightEnable)(THIS_ DWORD Index,BOOL Enable) PURE;
    STDMETHOD(GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable) PURE;
    STDMETHOD(SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane) PURE;
    STDMETHOD(GetClipPlane)(THIS_ DWORD Index,float* pPlane) PURE;
    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value) PURE;
    STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue) PURE;
    STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) PURE;
    STDMETHOD(BeginStateBlock)(THIS) PURE;
    STDMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) PURE;
    STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) PURE;
    STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) PURE;
    STDMETHOD(GetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture) PURE;
    STDMETHOD(SetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture) PURE;
    STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue) PURE;
    STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) PURE;
    STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) PURE;
    STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) PURE;
    STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) PURE;
    STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries) PURE;
    STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries) PURE;
    STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) PURE;
    STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber) PURE;
    STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) PURE;
    STDMETHOD(GetScissorRect)(THIS_ RECT* pRect) PURE;
    STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) PURE;
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) PURE;
    STDMETHOD(SetNPatchMode)(THIS_ float nSegments) PURE;
    STDMETHOD_(float, GetNPatchMode)(THIS) PURE;
    STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) PURE;
    STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) PURE;
    STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) PURE;
    STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) PURE;
    STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) PURE;
    STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) PURE;
    STDMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) PURE;
    STDMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) PURE;
    STDMETHOD(SetFVF)(THIS_ DWORD FVF) PURE;
    STDMETHOD(GetFVF)(THIS_ DWORD* pFVF) PURE;
    STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) PURE;
    STDMETHOD(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader) PURE;
    STDMETHOD(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader) PURE;
    STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) PURE;
    STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) PURE;
    STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) PURE;
    STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride) PURE;
    STDMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Setting) PURE;
    STDMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* pSetting) PURE;
    STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) PURE;
    STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) PURE;
    STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) PURE;
    STDMETHOD(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader) PURE;
    STDMETHOD(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader) PURE;
    STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) PURE;
    STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) PURE;
    STDMETHOD(DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) PURE;
    STDMETHOD(DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) PURE;
    STDMETHOD(DeletePatch)(THIS_ UINT Handle) PURE;
    STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) PURE;
};

class IDirect3DResource9 : public IDirect3DUnknown
{
protected:
	D3D9Base::LPDIRECT3DRESOURCE9 m_pResource;

public:
    IDirect3DResource9( D3D9Base::LPDIRECT3DRESOURCE9 pResource );
    static D3D9Wrapper::IDirect3DResource9* GetResource(D3D9Base::LPDIRECT3DRESOURCE9 pResource);
    inline D3D9Base::LPDIRECT3DRESOURCE9 GetResource9() { return m_pResource; }

    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
};

class IDirect3DBaseTexture9 : public IDirect3DResource9
{
protected:
    D3D9Base::LPDIRECT3DBASETEXTURE9 m_pBaseTexture;

public:
    IDirect3DBaseTexture9( D3D9Base::LPDIRECT3DBASETEXTURE9 pBaseTexture );
    static IDirect3DBaseTexture9* GetBaseTexture(D3D9Base::LPDIRECT3DBASETEXTURE9 pBaseTexture);
    inline D3D9Base::LPDIRECT3DBASETEXTURE9 GetBaseTexture9() { return m_pBaseTexture; }

    /*** IDirect3DBaseTexture9 methods ***/
    STDMETHOD_(DWORD, SetLOD)(THIS_ DWORD LODNew) PURE;
    STDMETHOD_(DWORD, GetLOD)(THIS) PURE;
    STDMETHOD_(DWORD, GetLevelCount)(THIS) PURE;
    STDMETHOD(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType) PURE;
    STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS) PURE;
    STDMETHOD_(void, GenerateMipSubLevels)(THIS) PURE;
};

class IDirect3DSwapChain9 : public IDirect3DUnknown
{
protected:
    D3D9Base::LPDIRECT3DSWAPCHAIN9 m_pSwapChain;
    D3D9Wrapper::IDirect3DDevice9* m_pDevice;
    static ThreadSafePointerSet	   m_List;

public:
    IDirect3DSwapChain9( D3D9Base::LPDIRECT3DSWAPCHAIN9 pSwapChain, D3D9Wrapper::IDirect3DDevice9* pDevice );

    /*** IUnknown methods ***/
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    static IDirect3DSwapChain9* GetSwapChain(D3D9Base::LPDIRECT3DSWAPCHAIN9 pSwapChain, D3D9Wrapper::IDirect3DDevice9* pDevice);
    inline D3D9Base::LPDIRECT3DSWAPCHAIN9 GetSwapChain9() { return m_pSwapChain; }

    /*** IDirect3DSwapChain9 methods ***/
    STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags) PURE;
    STDMETHOD(GetFrontBufferData)(THIS_ IDirect3DSurface9* pDestSurface) PURE;
    STDMETHOD(GetBackBuffer)(THIS_ UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) PURE;
    STDMETHOD(GetRasterStatus)(THIS_ D3DRASTER_STATUS* pRasterStatus) PURE;
    STDMETHOD(GetDisplayMode)(THIS_ D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(GetPresentParameters)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) PURE;
};

class IDirect3DTexture9 : public IDirect3DBaseTexture9
{
protected:
    D3D9Base::LPDIRECT3DTEXTURE9 m_pTexture;

public:
    IDirect3DTexture9( D3D9Base::LPDIRECT3DTEXTURE9 pTexture );

    /*** IDirect3DUnknown methods ***/
    STDMETHOD_(ULONG,Release)(THIS);

    static IDirect3DTexture9* GetTexture(D3D9Base::LPDIRECT3DTEXTURE9 pTexture);
    inline D3D9Base::LPDIRECT3DTEXTURE9 GetTexture9() { return m_pTexture; }

    STDMETHOD(GetLevelDesc)(THIS_ UINT Level,D3DSURFACE_DESC *pDesc) PURE;
    STDMETHOD(GetSurfaceLevel)(THIS_ UINT Level,IDirect3DSurface9** ppSurfaceLevel) PURE;
    STDMETHOD(LockRect)(THIS_ UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(THIS_ UINT Level) PURE;
    STDMETHOD(AddDirtyRect)(THIS_ CONST RECT* pDirtyRect) PURE;
};

class IDirect3DSurface9 : public IDirect3DResource9
{
protected:
    D3D9Base::LPDIRECT3DSURFACE9	m_pSurface;
    static ThreadSafePointerSet		m_List;

public:
	IDirect3DSurface9( D3D9Base::LPDIRECT3DSURFACE9 pSurface );
    static IDirect3DSurface9* GetSurface(D3D9Base::LPDIRECT3DSURFACE9 pSurface);
    inline D3D9Base::LPDIRECT3DSURFACE9 GetSurface9() { return m_pSurface; }

    /*** IDirect3DUnknown methods ***/
    STDMETHOD_(ULONG, Release)(THIS);

    /*** IDirect3DSurface8 methods ***/
    STDMETHOD(GetContainer)(THIS_ REFIID riid,void** ppContainer) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DSURFACE_DESC *pDesc) PURE;
    STDMETHOD(LockRect)(THIS_ D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(THIS) PURE;
    STDMETHOD(GetDC)(THIS_ HDC *phdc) PURE;
    STDMETHOD(ReleaseDC)(THIS_ HDC hdc) PURE;
};

class IDirect3DVertexBuffer9 : IDirect3DResource9
{
protected:
	D3D9Base::LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	BufferLockData m_LockData;
	bool Locked;

public:
    IDirect3DVertexBuffer9( D3D9Base::LPDIRECT3DVERTEXBUFFER9 pVertexBuffer );
    static IDirect3DVertexBuffer9* GetVertexBuffer( D3D9Base::LPDIRECT3DVERTEXBUFFER9 pVertexBuffer);
    inline D3D9Base::LPDIRECT3DVERTEXBUFFER9 GetVB9() { return m_pVertexBuffer; }

    STDMETHOD(Lock)(THIS_ UINT OffsetToLock,UINT SizeToLock,void** ppbData,DWORD Flags) PURE;
    STDMETHOD(Unlock)(THIS) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DVERTEXBUFFER_DESC *pDesc) PURE;
};

class IDirect3DIndexBuffer9 : IDirect3DResource9
{
protected:
	D3D9Base::LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	BufferLockData m_LockData;
	bool Locked;

public:
    IDirect3DIndexBuffer9( D3D9Base::LPDIRECT3DINDEXBUFFER9 pVertexBuffer );
    static IDirect3DIndexBuffer9* GetIndexBuffer( D3D9Base::LPDIRECT3DINDEXBUFFER9 pVertexBuffer);
    inline D3D9Base::LPDIRECT3DINDEXBUFFER9 GetIB9() { return m_pIndexBuffer; }

    STDMETHOD(Lock)(THIS_ UINT OffsetToLock,UINT SizeToLock,void** ppbData,DWORD Flags) PURE;
    STDMETHOD(Unlock)(THIS) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DINDEXBUFFER_DESC *pDesc) PURE;
};
