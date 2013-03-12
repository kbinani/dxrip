/*
D3D9Wrapper.cpp
Written by Matthew Fisher

Contains the implementation of all wrapped D3D9 functions.
*/

#include "Main.h"

//TODO: ReportSetStreamSource -> use new VBuffer

//http://www.gamerzplanet.net/forums/soldier-front-hacks-discussion/202750-how-to-move-system-files-in-vista-support-thread.html
//takeown /f c:\windows\system32\
//icacls c:\windows\system32\ /grant mdfisher:F

//Disable data execution prevention
//bcdedit.exe /set {current} nx AlwaysOff

#ifdef USE_EMPTY_CALLBACK
#include "d3d9CallbackEmpty.h"
#else
#include "d3d9Callback.h"
#endif

const UINT ScratchSurfaceHeight = 2048;
const UINT ScratchSurfaceWidth = 2048;
const UINT MaxSurfaceLevels = 12;
const bool UpdateDynamicTextures = false;
const bool IgnoringBufferLocks = false;
const bool InitialMessageBoxBreak = false;
const bool DebuggingSurfaceCreation = false;
const bool ForceMultithreadedDevice = false;
const bool SkipSetRenderState = true;
const bool SkipSetViewport = true;
const bool SkipVertexBufferConstants = false;
const bool SkipSetVertexDeclaration = false;
const bool SkipSetIndices = false;
const bool SkipSetStreamSource = false;

Globals g_Globals;

ThreadSafePointerSet D3D9Wrapper::IDirect3DSwapChain9::m_List;
ThreadSafePointerSet D3D9Wrapper::IDirect3DSurface9::m_List;
ThreadSafePointerSet D3D9Wrapper::IDirect3DDevice9::m_List;
ThreadSafePointerSet D3D9Wrapper::IDirect3DDevice9::m_DeclList;
ThreadSafePointerSet D3D9Wrapper::IDirect3DDevice9::m_VertexShaderList;
ThreadSafePointerSet D3D9Wrapper::IDirect3DDevice9::m_PixelShaderList;
ThreadSafePointerSet D3D9Wrapper::IDirect3D9::m_List;

void InitializeDLL()
{
	if(!g_Globals.Initialized)
	{
        if(InitialMessageBoxBreak)
        {
            MessageBox(NULL, "D3D9 Wrapper Loaded", "D3D9Wrapper", MB_OK);
        }
        g_Globals.Init();
        g_Globals.InfoFile() << "Initializing DLL...\n";
		D3D9CallbackInitialize();
		g_Globals.InfoFile() << "DLL initialized.\n";
	}
}

void DestroyDLL()
{
	if(g_Globals.Initialized)
	{
		g_Globals.InfoFile() << "Destroying DLL...\n";
		D3D9CallbackFreeMemory();
	}
}

int WINAPI D3DPERF_BeginEvent(DWORD col, LPCWSTR wszName)
{
    return 0;
}

int WINAPI D3DPERF_EndEvent()
{
    return 0;
}

void WINAPI D3DPERF_SetMarker()
{
    MessageBox(NULL, "D3DPERF_SetMarker", "D3D9Wrapper", MB_OK);
}

void WINAPI D3DPERF_SetRegion()
{
    MessageBox(NULL, "D3DPERF_SetRegion", "D3D9Wrapper", MB_OK);
}

void WINAPI D3DPERF_QueryRepeatFrame()
{
    MessageBox(NULL, "D3DPERF_QueryRepeatFrame", "D3D9Wrapper", MB_OK);
}

void WINAPI D3DPERF_SetOptions(DWORD options)
{
    //MessageBox(NULL, "D3DPERF_SetOptions", "D3D9Wrapper", MB_OK);
}

void WINAPI D3DPERF_GetStatus()
{
    MessageBox(NULL, "D3DPERF_GetStatus", "D3D9Wrapper", MB_OK);
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
bool IsWow64()
{
    BOOL Result = FALSE;

    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &Result))
        {
			g_Globals.ErrorFile() << "fnIsWow64Process failed\n";
        }
    }
    return (Result != FALSE);
}

D3D9Wrapper::IDirect3D9* WINAPI Direct3DCreate9(UINT Version)
{
	InitializeDLL();
	HMODULE hD3D;
	if(IsWow64())
	{
		hD3D = LoadLibrary("C:\\Windows\\SysWOW64\\d3d9.dll");
	}
	else
	{
		hD3D = LoadLibrary(g_Globals.RealD3D9DLL.CString());
	}
	
    if( hD3D == NULL )
    {
        g_Globals.ErrorFile() << "LoadLibrary on d3d9.dll failed\n";
        return NULL;
    }

	D3D9Wrapper::D3DCREATE pCreate = (D3D9Wrapper::D3DCREATE)GetProcAddress(hD3D, _T("Direct3DCreate9"));
    if( pCreate == NULL )
    {
        g_Globals.ErrorFile() << "coud not find Direct3DCreate9 in d3d9.dll\n";
        return NULL;
    }

	D3D9Base::LPDIRECT3D9 pD3D = pCreate(D3D_SDK_VERSION);
    if( pD3D == NULL )
    {
        return NULL;
    }
    
	//return (D3D9Wrapper::IDirect3D9*)pD3D;
    return D3D9Wrapper::IDirect3D9::GetDirect3D(pD3D);
}

#include "Direct3D9Functions.h"
#include "Direct3DDevice9Functions.h"
#include "Direct3DSwapChain9Functions.h"
#include "Direct3DResource9Functions.h"
#include "Direct3DBaseTexture9Functions.h"
#include "Direct3DTexture9Functions.h"
#include "Direct3DSurface9Functions.h"
#include "Direct3DVertexBuffer9Functions.h"
#include "Direct3DIndexBuffer9Functions.h"

//
// Check up on UpdateTexture/UpdateSurface
//