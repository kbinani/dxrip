D3D9Wrapper::IDirect3DSwapChain9::IDirect3DSwapChain9( D3D9Base::LPDIRECT3DSWAPCHAIN9 pSwapChain, D3D9Wrapper::IDirect3DDevice9* pDevice )
    : IDirect3DUnknown((IUnknown*) pSwapChain)
{
    m_pSwapChain = pSwapChain;
    m_pDevice = pDevice;
}

D3D9Wrapper::IDirect3DSwapChain9* D3D9Wrapper::IDirect3DSwapChain9::GetSwapChain(D3D9Base::LPDIRECT3DSWAPCHAIN9 pSwapChain, D3D9Wrapper::IDirect3DDevice9* pDevice )
{
    D3D9Wrapper::IDirect3DSwapChain9* p = (D3D9Wrapper::IDirect3DSwapChain9*) m_List.GetDataPtr(pSwapChain);
    if( p == NULL )
    {
        p = new D3D9Wrapper::IDirect3DSwapChain9(pSwapChain, pDevice);
        m_List.AddMember(pSwapChain, p);
        return p;
    }
    
    p->m_ulRef++;
    return p;
}

STDMETHODIMP_(ULONG) D3D9Wrapper::IDirect3DSwapChain9::Release(THIS)
{
	m_pUnk->Release();

    ULONG ulRef = --m_ulRef;
    if(ulRef == 0)
    {
        m_List.DeleteMember(GetSwapChain9());
        delete this;
        return 0;
    }
    return ulRef;
}

STDMETHODIMP D3D9Wrapper::IDirect3DSwapChain9::Present(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)
{
    ReportPresent(pSourceRect, pDestRect);

    if(g_Globals.UsingOverlay)
    {
        D3D9Base::IDirect3DDevice9 *Device;
        m_pSwapChain->GetDevice(&Device);
        Device->BeginScene();
        //D3D9Wrapper::IDirect3DDevice9::GetDirect3DDevice(Device)->GetOverlay().RenderConsole();
        m_pDevice->GetOverlay().RenderConsole();
	    Device->EndScene();
        Device->Release();
    }

	HRESULT hr = m_pSwapChain->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	if(hr == D3DERR_DEVICELOST)
	{
        D3D9Base::IDirect3DDevice9 *Device;
        m_pSwapChain->GetDevice(&Device);
        //D3D9Wrapper::IDirect3DDevice9::GetDirect3DDevice(Device)->GetOverlay().FreeMemory();
        m_pDevice->GetOverlay().FreeMemory();
        Device->Release();
		//m_Console.OnLostDevice();
	}
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DSwapChain9::GetFrontBufferData(THIS_ IDirect3DSurface9* pDestSurface)
{
    g_Globals.ErrorFile() << "IDirect3DSwapChain9::GetFrontBufferData not supported\n";
    return E_OUTOFMEMORY;
	//return m_pSwapChain->GetFrontBufferData(pDestSurface);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSwapChain9::GetBackBuffer(THIS_ UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	*ppBackBuffer = NULL;
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;

    HRESULT hr = m_pSwapChain->GetBackBuffer(iBackBuffer, Type, &BaseSurface);
    if(FAILED(hr) || BaseSurface == NULL)
    {
        return hr;
    }
    
    D3D9Wrapper::IDirect3DSurface9* NewSurface = D3D9Wrapper::IDirect3DSurface9::GetSurface(BaseSurface);

    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "GetBackBuffer called. D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(NewSurface)) << endl;
    }

    if(NewSurface == NULL)
    {
        BaseSurface->Release();
        return E_OUTOFMEMORY;
    }
    *ppBackBuffer = NewSurface;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DSwapChain9::GetRasterStatus(THIS_ D3DRASTER_STATUS* pRasterStatus)
{
	return m_pSwapChain->GetRasterStatus(pRasterStatus);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSwapChain9::GetDisplayMode(THIS_ D3DDISPLAYMODE* pMode)
{
	return m_pSwapChain->GetDisplayMode(pMode);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSwapChain9::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
    g_Globals.ErrorFile() << "IDirect3DSwapChain9::GetDevice not supported\n";
    return E_OUTOFMEMORY;
    //return m_pSwapChain->GetFrontBufferData(pDestSurface);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSwapChain9::GetPresentParameters(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return m_pSwapChain->GetPresentParameters(pPresentationParameters);
}
