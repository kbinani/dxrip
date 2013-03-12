
D3D9Wrapper::IDirect3D9::IDirect3D9(D3D9Base::LPDIRECT3D9 pD3D)
    : D3D9Wrapper::IDirect3DUnknown((IUnknown*) pD3D)
{
    m_pD3D = pD3D;
}

D3D9Wrapper::IDirect3D9* D3D9Wrapper::IDirect3D9::GetDirect3D(D3D9Base::LPDIRECT3D9 pD3D)
{
    D3D9Wrapper::IDirect3D9* p = (D3D9Wrapper::IDirect3D9*) m_List.GetDataPtr(pD3D);
    if( p == NULL )
    {
        p = new D3D9Wrapper::IDirect3D9(pD3D);
        m_List.AddMember(pD3D,p);
        return p;
    }
    
    p->m_ulRef++;
    return p;
}

STDMETHODIMP_(ULONG) D3D9Wrapper::IDirect3D9::Release(THIS)
{
    m_pUnk->Release();

    ULONG ulRef = --m_ulRef;

    if(ulRef == 0)
    {
        m_List.DeleteMember(GetDirect3D9());
        delete this;
        return 0L;
    }
    return ulRef;
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::RegisterSoftwareDevice(THIS_ void* pInitializeFunction)
{
	return m_pD3D->RegisterSoftwareDevice(pInitializeFunction);
}

STDMETHODIMP_(UINT) D3D9Wrapper::IDirect3D9::GetAdapterCount(THIS)
{
	return m_pD3D->GetAdapterCount();
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::GetAdapterIdentifier(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	return m_pD3D->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

STDMETHODIMP_(UINT) D3D9Wrapper::IDirect3D9::GetAdapterModeCount(THIS_ UINT Adapter,D3DFORMAT Format)
{
	return m_pD3D->GetAdapterModeCount(Adapter, Format);
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::EnumAdapterModes(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode)
{
	HRESULT hr = m_pD3D->EnumAdapterModes(Adapter, Format, Mode, pMode);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::GetAdapterDisplayMode(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode)
{
	HRESULT hr = m_pD3D->GetAdapterDisplayMode(Adapter, pMode);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::CheckDeviceType(THIS_ UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed)
{
	HRESULT hr = m_pD3D->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::CheckDeviceFormat(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat)
{
	Assert(Adapter == 0, "Non-zero adapter");
	HRESULT hr = m_pD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::CheckDeviceMultiSampleType(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
	return m_pD3D->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::CheckDepthStencilMatch(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
{
	HRESULT hr = m_pD3D->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::CheckDeviceFormatConversion(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat)
{
	HRESULT hr = m_pD3D->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::GetDeviceCaps(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps)
{
	HRESULT hr = m_pD3D->GetDeviceCaps(Adapter, DeviceType, pCaps);
	//Assert(hr != D3DERR_NOTAVAILABLE, "D3DERR_NOTAVAILABLE");
	return hr;
}

STDMETHODIMP_(HMONITOR) D3D9Wrapper::IDirect3D9::GetAdapterMonitor(THIS_ UINT Adapter)
{
	return m_pD3D->GetAdapterMonitor(Adapter);
}

STDMETHODIMP D3D9Wrapper::IDirect3D9::CreateDevice(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,D3D9Wrapper::IDirect3DDevice9** ppReturnedDeviceInterface)
{
	g_Globals.InfoFile() << "IDirect3D9::CreateDevice\n";
	g_Globals.InfoFile() << "BackBufferWidth " << pPresentationParameters->BackBufferWidth << endl;
	g_Globals.InfoFile() << "BackBufferHeight " << pPresentationParameters->BackBufferHeight << endl;
	g_Globals.InfoFile() << "BackBufferFormat " << pPresentationParameters->BackBufferFormat << endl;
	g_Globals.InfoFile() << "BackBufferCount " << pPresentationParameters->BackBufferCount << endl;
	g_Globals.InfoFile() << "SwapEffect " << pPresentationParameters->SwapEffect << endl;
	g_Globals.InfoFile() << "Flags " << pPresentationParameters->Flags << endl;
	g_Globals.InfoFile() << "FullScreen_RefreshRateInHz " << pPresentationParameters->FullScreen_RefreshRateInHz << endl;
	g_Globals.InfoFile() << "PresentationInterval " << pPresentationParameters->PresentationInterval << endl;

    if(g_Globals.ForceSoftwareVertexProcessing)
	{
		BehaviorFlags &= ~D3DCREATE_MIXED_VERTEXPROCESSING;
		BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
    if(ForceMultithreadedDevice)
    {
        BehaviorFlags |= D3DCREATE_MULTITHREADED;
    }

	D3D9Base::LPDIRECT3DDEVICE9 BaseDevice = NULL;
	HRESULT hr = m_pD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &BaseDevice);
	if(FAILED(hr))
	{
		*ppReturnedDeviceInterface = NULL;
        return hr;
	}

	D3D9Wrapper::IDirect3DDevice9* NewDevice = D3D9Wrapper::IDirect3DDevice9::GetDirect3DDevice(BaseDevice);
	if(NewDevice == NULL)
	{
		BaseDevice->Release();
		return E_OUTOFMEMORY;
	}
	
    ReportCreateDevice(BaseDevice, &(NewDevice->GetOverlay()));
	*ppReturnedDeviceInterface = NewDevice;
	//*ppReturnedDeviceInterface = (D3D9Wrapper::IDirect3DDevice9*)BaseDevice;
	return hr;
}

