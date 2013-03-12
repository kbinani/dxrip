
void D3D9Wrapper::IDirect3DDevice9::BitmapFromSurface(D3D9Base::LPDIRECT3DSURFACE9 pSurface, const RECT &Rect, Bitmap &Bmp)
{
	D3DSURFACE_DESC Desc;
	D3DLOCKED_RECT LockedRect;
	HRESULT hr = pSurface->GetDesc(&Desc);
	Assert(SUCCEEDED(hr), "pSurface->GetDesc failed");
	//m_pSurface->GetDesc(&LD.Desc);
	//DestSurface = D3D9Wrapper::IDirect3DDevice9::GetDirect3DDevice(m_pDevice)->GetOffscreenSurface();
	/*RECT Rect;
	Rect.left = 0;
	Rect.right = Desc.Width;
	Rect.top = 0;
	Rect.bottom = Desc.Height;*/
#ifdef USE_D3DX
	if(pSurface != m_pScratchSurface)
	{
		hr = D3D9Base::D3DXLoadSurfaceFromSurface(m_pScratchSurface, NULL, &Rect, pSurface, NULL, &Rect, D3DX_FILTER_POINT, D3DCOLOR_XRGB(0, 0, 0));
        if(FAILED(hr))
        {
            Bmp.Allocate(1, 1);
            Bmp[0][0] = RGBColor::Magenta;
            return;
        }
		Assert(SUCCEEDED(hr), "D3DXLoadSurfaceFromSurface failed");
	}
	hr = m_pScratchSurface->LockRect(&LockedRect, &Rect, NULL);
	Assert(SUCCEEDED(hr), "m_pScratchSurface->LockRect failed");
	
	Bmp.Allocate(Desc.Width, Desc.Height);
	RGBColor *SurfData = (RGBColor*)LockedRect.pBits;
    for(UINT y = 0; y < Desc.Height; y++)
    {
        memcpy(Bmp[Desc.Height - 1 - y], &SurfData[y * LockedRect.Pitch / 4], Desc.Width * 4);
    }

	hr = m_pScratchSurface->UnlockRect();
	Assert(SUCCEEDED(hr), "m_pScratchSurface->UnlockRect failed");
#endif
}

D3D9Wrapper::IDirect3DDevice9::IDirect3DDevice9(D3D9Base::LPDIRECT3DDEVICE9 pDevice)
    : IDirect3DUnknown((IUnknown*) pDevice)
{
    m_pDevice = pDevice;
	g_Globals.InfoFile() << "Creating objects\n";
	m_pDevice->CreateOffscreenPlainSurface(ScratchSurfaceWidth, ScratchSurfaceHeight, D3D9Base::D3DFMT_A8R8G8B8, D3D9Base::D3DPOOL_SYSTEMMEM, &m_pScratchSurface, NULL);
	m_Overlay.Init(m_pDevice);
}

D3D9Wrapper::IDirect3DDevice9* D3D9Wrapper::IDirect3DDevice9::GetDirect3DDevice(D3D9Base::LPDIRECT3DDEVICE9 pDevice)
{
    D3D9Wrapper::IDirect3DDevice9* p = (D3D9Wrapper::IDirect3DDevice9*) m_List.GetDataPtr(pDevice);
    if(p == NULL)
    {
        p = new D3D9Wrapper::IDirect3DDevice9(pDevice);
        m_List.AddMember(pDevice, p);
        return p;
    }
    
    p->m_ulRef++;
    return p;
}

STDMETHODIMP_(ULONG) D3D9Wrapper::IDirect3DDevice9::Release(THIS)
{
	//g_Globals.InfoFile() << "Releasing device: " << m_ulRef - 1 << endl;
	if(m_ulRef == 1)
	{
		ReportFreeDevice();
		m_pScratchSurface->Release();
		m_Overlay.FreeMemory();
		g_Globals.InfoFile() << "Releasing objects\n";
	}

    m_pUnk->Release();

    ULONG ulRef = --m_ulRef;

    if(ulRef == 0)
    {
        m_List.DeleteMember(GetD3D9Device());
        delete this;
        return NULL;
    }
    return ulRef;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::TestCooperativeLevel(THIS)
{
	return m_pDevice->TestCooperativeLevel();
}

STDMETHODIMP_(UINT) D3D9Wrapper::IDirect3DDevice9::GetAvailableTextureMem(THIS)
{
	return m_pDevice->GetAvailableTextureMem();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::EvictManagedResources(THIS)
{
	return m_pDevice->EvictManagedResources();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetDirect3D(THIS_ D3D9Wrapper::IDirect3D9** ppD3D9)
{
    D3D9Base::LPDIRECT3D9 BaseDirect3D = NULL;
    HRESULT hr = m_pDevice->GetDirect3D(&BaseDirect3D);
    if(FAILED(hr))
    {
        *ppD3D9 = NULL;
        return hr;
    }

	D3D9Wrapper::IDirect3D9* pD3D = D3D9Wrapper::IDirect3D9::GetDirect3D(BaseDirect3D);
    if(pD3D == NULL)
    {    
        BaseDirect3D->Release();
        hr = E_OUTOFMEMORY;
    }
    *ppD3D9 = pD3D;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetDeviceCaps(THIS_ D3DCAPS9* pCaps)
{
	HRESULT hr = m_pDevice->GetDeviceCaps(pCaps);
	//Assert(hr != D3DERR_NOTAVAILABLE, "D3DERR_NOTAVAILABLE");
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetDisplayMode(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode)
{
	return m_pDevice->GetDisplayMode(iSwapChain, pMode);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetCreationParameters(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return m_pDevice->GetCreationParameters( pParameters);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetCursorProperties(THIS_ UINT XHotSpot,UINT YHotSpot,D3D9Wrapper::IDirect3DSurface9* pCursorBitmap)
{
	return m_pDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap == 0 ? 0 : pCursorBitmap->GetSurface9());
}

STDMETHODIMP_(void) D3D9Wrapper::IDirect3DDevice9::SetCursorPosition(THIS_ int X,int Y,DWORD Flags)
{
	return m_pDevice->SetCursorPosition(X, Y, Flags);
}

STDMETHODIMP_(BOOL) D3D9Wrapper::IDirect3DDevice9::ShowCursor(THIS_ BOOL bShow)
{
	return m_pDevice->ShowCursor(bShow);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateAdditionalSwapChain(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{
	g_Globals.UnreportedFile() << "CreateAdditionalSwapChain\n";
    return E_OUTOFMEMORY;
	//return m_pDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetSwapChain(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
    *pSwapChain = NULL;
	D3D9Base::LPDIRECT3DSWAPCHAIN9 BaseSwapChain = NULL;
    HRESULT hr = m_pDevice->GetSwapChain(iSwapChain, &BaseSwapChain);
    if(FAILED(hr) || BaseSwapChain == NULL)
    {
        return hr;
    }
    
    D3D9Wrapper::IDirect3DSwapChain9* NewSwapChain = D3D9Wrapper::IDirect3DSwapChain9::GetSwapChain(BaseSwapChain, this);

    if(NewSwapChain == NULL)
    {
        BaseSwapChain->Release();
        return E_OUTOFMEMORY;
    }
    *pSwapChain = NewSwapChain;
    return hr;
}

STDMETHODIMP_(UINT) D3D9Wrapper::IDirect3DDevice9::GetNumberOfSwapChains(THIS)
{
	return m_pDevice->GetNumberOfSwapChains();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::Reset(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	//m_Console.OnLostDevice();
    m_Overlay.FreeMemory();
	HRESULT hr = m_pDevice->Reset(pPresentationParameters);
	if(SUCCEEDED(hr))
	{
        m_Overlay.Init(m_pDevice);
		//m_Console.OnResetDevice();
	}
	return hr;
}

UINT FrameIndex = 0;
STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::Present(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	ReportPresent(pSourceRect, pDestRect);

    if(g_Globals.UsingOverlay)
    {
        m_pDevice->BeginScene();
	    m_Overlay.RenderConsole();
	    m_pDevice->EndScene();
    }

	/*if(g_Globals.CaptureNextFullScreen)
	{
		g_Globals.CaptureNextFullScreen = false;
		Bitmap Bmp;
		D3D9Base::LPDIRECT3DSURFACE9 BackBuffer;
		HRESULT hr = m_pDevice->GetBackBuffer(0, 0, D3D9Base::D3DBACKBUFFER_TYPE_MONO, &BackBuffer);
		Assert(SUCCEEDED(hr), "D3DDevice->GetBackBuffer failed");
		BitmapFromSurface(BackBuffer, Bmp);
		BackBuffer->Release();
		Bmp.SavePNG(g_Globals.NewTextureDirectory + String("ScreenCapture.png"));
	}*/

	HRESULT hr = m_pDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	if(hr == D3DERR_DEVICELOST)
	{
        m_Overlay.FreeMemory();
		//m_Console.OnLostDevice();
	}
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetBackBuffer(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,D3D9Wrapper::IDirect3DSurface9** ppBackBuffer)
{
	*ppBackBuffer = NULL;
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;

    HRESULT hr = m_pDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, &BaseSurface);
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

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetRasterStatus(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
	return m_pDevice->GetRasterStatus(iSwapChain, pRasterStatus);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetDialogBoxMode(THIS_ BOOL bEnableDialogs)
{
	return m_pDevice->SetDialogBoxMode(bEnableDialogs);
}

STDMETHODIMP_(void) D3D9Wrapper::IDirect3DDevice9::SetGammaRamp(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	return m_pDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}

STDMETHODIMP_(void) D3D9Wrapper::IDirect3DDevice9::GetGammaRamp(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
	return m_pDevice->GetGammaRamp(iSwapChain, pRamp);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateTexture(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
	*ppTexture = NULL;
	D3D9Base::LPDIRECT3DTEXTURE9 BaseTexture = NULL;
    HRESULT hr = m_pDevice->CreateTexture( Width, Height, Levels, Usage, Format, Pool, &BaseTexture, pSharedHandle);
    if( FAILED(hr) )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DTexture9* NewTexture = D3D9Wrapper::IDirect3DTexture9::GetTexture(BaseTexture);
    if(NewTexture == NULL)
    {
        BaseTexture->Release();
        return E_OUTOFMEMORY;
    }
	else
	{
		*ppTexture = NewTexture;

		//
		// Report the texture create
		//
		D3D9Base::LPDIRECT3DSURFACE9 TopLevelSurface;
		BaseTexture->GetSurfaceLevel(0, &TopLevelSurface);

        //g_Globals.ErrorFile() << "CreateTexture BaseTexture=" << BaseTexture << " TopLevelSurface=" << TopLevelSurface << endl;

		D3DSURFACE_DESC Desc;
		TopLevelSurface->GetDesc(&Desc);
		Bitmap Bmp(Desc.Width, Desc.Height);
        Bmp.Clear(RGBColor::Magenta);
		ReportUnlockTexture(Desc, Bmp, BaseTexture);
		TopLevelSurface->Release();
		return hr;
	}
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateVolumeTexture(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
	g_Globals.UnreportedFile() << "CreateVolumeTexture\n";
	return m_pDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateCubeTexture(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{
	g_Globals.UnreportedFile() << "CreateCubeTexture\n";
	return m_pDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateVertexBuffer(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{
	*ppVertexBuffer = NULL;
	D3D9Base::LPDIRECT3DVERTEXBUFFER9 BaseVB = NULL;

    if(g_Globals.ForceSoftwareVertexProcessing)
    {
        Usage |= D3DUSAGE_SOFTWAREPROCESSING;
    }

    HRESULT hr = m_pDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, &BaseVB, pSharedHandle);
    if( FAILED(hr) )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DVertexBuffer9* NewVB = D3D9Wrapper::IDirect3DVertexBuffer9::GetVertexBuffer(BaseVB);
    if(NewVB == NULL)
    {
        BaseVB->Release();
        return E_OUTOFMEMORY;
    }
    *ppVertexBuffer = NewVB;

    BufferLockData LD;
    LD.Handle = BaseVB;
    LD.Flags = 0;
    LD.OffsetToLock = 0;
    LD.SizeToLock = 0;
    LD.pRAMBuffer = NULL;
    LD.Create = true;
    D3DVERTEXBUFFER_DESC Desc;
    BaseVB->GetDesc(&Desc);
    ReportLockVertexBuffer(LD, Desc);
    
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateIndexBuffer(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
	*ppIndexBuffer = NULL;
	D3D9Base::LPDIRECT3DINDEXBUFFER9 BaseIB = NULL;
    HRESULT hr = m_pDevice->CreateIndexBuffer(Length, Usage, Format, Pool, &BaseIB, pSharedHandle);
    if( FAILED(hr) )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DIndexBuffer9* NewIB = D3D9Wrapper::IDirect3DIndexBuffer9::GetIndexBuffer(BaseIB);
    if(NewIB == NULL)
    {
        BaseIB->Release();
        return E_OUTOFMEMORY;
    }
    *ppIndexBuffer = NewIB;

    BufferLockData LD;
    LD.Handle = BaseIB;
    LD.Flags = 0;
    LD.OffsetToLock = 0;
    LD.SizeToLock = 0;
    LD.pRAMBuffer = NULL;
    LD.Create = true;
    D3DINDEXBUFFER_DESC Desc;
    BaseIB->GetDesc(&Desc);
    ReportLockIndexBuffer(LD, Desc);

    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateRenderTarget(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	*ppSurface = NULL;
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;
    HRESULT hr = m_pDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &BaseSurface, pSharedHandle);
    if( FAILED(hr) )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DSurface9* NewSurface = D3D9Wrapper::IDirect3DSurface9::GetSurface(BaseSurface);

    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "CreateRenderTarget called. D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(NewSurface)) << endl;
    }

    if(NewSurface == NULL)
    {
        BaseSurface->Release();
        return E_OUTOFMEMORY;
    }
    *ppSurface = NewSurface;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateDepthStencilSurface(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	*ppSurface = NULL;
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;
    HRESULT hr = m_pDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, &BaseSurface, pSharedHandle);
    if( FAILED(hr) )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DSurface9* NewSurface = D3D9Wrapper::IDirect3DSurface9::GetSurface(BaseSurface);

    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "CreateDepthStencilSurface called. D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(NewSurface)) << endl;
    }

    if(NewSurface == NULL)
    {
        BaseSurface->Release();
        return E_OUTOFMEMORY;
    }
    *ppSurface = NewSurface;
    return hr;
}

void ReportUpdateSurface(D3D9Wrapper::IDirect3DDevice9 *Device, D3D9Wrapper::IDirect3DSurface9* pSourceSurface, const RECT &SourceRect, D3DSURFACE_DESC &DestinationDesc, D3D9Base::LPDIRECT3DTEXTURE9 BaseTexture)
{
    Bitmap Bmp;
    Device->BitmapFromSurface(pSourceSurface->GetSurface9(), SourceRect, Bmp);
    ReportUnlockTexture(DestinationDesc, Bmp, BaseTexture);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::UpdateSurface(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
	D3D9Base::LPDIRECT3DSURFACE9 BaseSourceSurface = NULL;
	if(pSourceSurface != NULL)
	{
		BaseSourceSurface = pSourceSurface->GetSurface9();
	}
	D3D9Base::LPDIRECT3DSURFACE9 BaseDestinationSurface = NULL;
	if(pDestinationSurface != NULL)
	{
		BaseDestinationSurface = pDestinationSurface->GetSurface9();
	}
    //g_Globals.InfoFile() << "UpdateSurface called\n";
    HRESULT hr = m_pDevice->UpdateSurface(BaseSourceSurface, pSourceRect, BaseDestinationSurface, pDestPoint);
    if(SUCCEEDED(hr))
    {
        D3D9Base::LPDIRECT3DTEXTURE9 BaseTexture = NULL;
        hr = pDestinationSurface->GetContainer(D3D9Base::IID_IDirect3DTexture9, (void **)&BaseTexture);
        if(SUCCEEDED(hr))
        {
            D3DSURFACE_DESC DestinationDesc, TopLevelDesc;
            pDestinationSurface->GetDesc(&DestinationDesc);
            /*UINT IsRenderTarget = 0;
            if(MyDesc.Usage & D3DUSAGE_RENDERTARGET)
            {
                IsRenderTarget = 1;
            }
            g_Globals.InfoFile() << "UpdateSurface Desc: Size=" << MyDesc.Width << "x" << MyDesc.Height << "Usage=" << IsRenderTarget << endl;*/

            hr = BaseTexture->GetLevelDesc(0, &TopLevelDesc);
            Assert(SUCCEEDED(hr), "BaseTexture->GetLevelDesc failed");
            //g_Globals.InfoFile() << "TopLevel check: " << this << " TopLevel=" << TopLevelDesc.Width << "x" << TopLevelDesc.Height << " this=" << MyDesc.Width << "x" << MyDesc.Height << endl;
            if(TopLevelDesc.Width == DestinationDesc.Width && TopLevelDesc.Height == DestinationDesc.Height)
            {
                ReportUpdateSurface(this, pSourceSurface, *pSourceRect, DestinationDesc, BaseTexture);
            }
            BaseTexture->Release();
        }
        else
        {
            g_Globals.ErrorFile() << "GetContainer failed\n"; //(could be a cubemap)
        }
    }
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::UpdateTexture(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
    g_Globals.InfoFile() << "UpdateTexture called\n";
	return m_pDevice->UpdateTexture(pSourceTexture->GetBaseTexture9(), pDestinationTexture->GetBaseTexture9());
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetRenderTargetData(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
	D3D9Base::LPDIRECT3DSURFACE9 BaseRenderTarget = NULL;
	if(pRenderTarget != NULL)
	{
		BaseRenderTarget = pRenderTarget->GetSurface9();
	}
	return m_pDevice->GetRenderTargetData(BaseRenderTarget, pDestSurface->GetSurface9());
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetFrontBufferData(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
	D3D9Base::LPDIRECT3DSURFACE9 BaseDestSurface = NULL;
	if(pDestSurface != NULL)
	{
		BaseDestSurface = pDestSurface->GetSurface9();
	}
	return m_pDevice->GetFrontBufferData(iSwapChain, BaseDestSurface);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::StretchRect(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
    g_Globals.InfoFile() << "StretchRect called\n";
	return m_pDevice->StretchRect(pSourceSurface->GetSurface9(), pSourceRect, pDestSurface->GetSurface9(), pDestRect, Filter);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::ColorFill(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	return m_pDevice->ColorFill(pSurface->GetSurface9(), pRect, color);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateOffscreenPlainSurface(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,D3D9Wrapper::IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	*ppSurface = NULL;
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;
    HRESULT hr = m_pDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, &BaseSurface, pSharedHandle);
    if( FAILED(hr) )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DSurface9* NewSurface = D3D9Wrapper::IDirect3DSurface9::GetSurface(BaseSurface);

    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "CreateOffscreenPlainSurface called. D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(NewSurface)) << endl;
    }

    if(NewSurface == NULL)
    {
        BaseSurface->Release();
        return E_OUTOFMEMORY;
    }
    *ppSurface = NewSurface;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetRenderTarget(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
	/*IDirect3DSurface9* pOldRenderTarget = NULL;
	if(UpdateDynamicTextures)
	{
		HRESULT hr = GetRenderTarget(RenderTargetIndex, &pOldRenderTarget);
		if( FAILED(hr) )
		{
			g_Globals.ErrorFile() << "GetRenderTarget failed in SetRenderTarget\n";
			return hr;
		}
	}*/

    D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;
    if(pRenderTarget != NULL)
    {
        BaseSurface = pRenderTarget->GetSurface9();
    }
	ReportSetRenderTarget(RenderTargetIndex, BaseSurface);

    //
    // HACK: because I don't reimplement IDirect3DSwapChain9, and return the D3D9Base swap chain, it is possible that pRenderTarget
    // is a D3D9Base object, not a D3D9Wrapper one
    //
    if(UINT(BaseSurface) == 1)
    {
        g_Globals.InfoFile() << "D3D9Base render target detected (IDirect3DSwapChain9 bug)\n";
        BaseSurface = (D3D9Base::LPDIRECT3DSURFACE9)pRenderTarget;
    }
    
    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "SetRenderTarget called. D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(pRenderTarget)) << endl;
    }

	HRESULT hr = m_pDevice->SetRenderTarget(RenderTargetIndex, BaseSurface);
	if(FAILED(hr))
	{
		return hr;
	}
	/*if(UpdateDynamicTextures && pOldRenderTarget != NULL)
	{
		D3DLOCKED_RECT LockInfo;
		hr = pOldRenderTarget->LockRect(&LockInfo, NULL, 0);
		if(FAILED(hr))
		{
			g_Globals.ErrorFile() << "pOldRenderTarget->LockRect failed in SetRenderTarget\n";
			return hr;
		}
		hr = pOldRenderTarget->UnlockRect();
		if(FAILED(hr))
		{
			g_Globals.ErrorFile() << "pOldRenderTarget->UnlockRect failed in SetRenderTarget\n";
			return hr;
		}
	}*/
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetRenderTarget(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	*ppRenderTarget = NULL;
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;
    HRESULT hr = m_pDevice->GetRenderTarget(RenderTargetIndex, &BaseSurface);
    if(FAILED(hr) || BaseSurface == NULL)
    {
        return hr;
    }
    
    D3D9Wrapper::IDirect3DSurface9* NewSurface = D3D9Wrapper::IDirect3DSurface9::GetSurface(BaseSurface);

    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "GetRenderTarget called. D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(NewSurface)) << endl;
    }

    if(NewSurface == NULL)
    {
        BaseSurface->Release();
        return E_OUTOFMEMORY;
    }
    *ppRenderTarget = NewSurface;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetDepthStencilSurface(THIS_ IDirect3DSurface9* pNewZStencil)
{
	if(pNewZStencil == NULL)
	{
		return m_pDevice->SetDepthStencilSurface(NULL);
	}
	else
	{
		return m_pDevice->SetDepthStencilSurface(pNewZStencil->GetSurface9());
	}
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetDepthStencilSurface(THIS_ IDirect3DSurface9** ppZStencilSurface)
{
	*ppZStencilSurface = NULL;
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;
    HRESULT hr = m_pDevice->GetDepthStencilSurface(&BaseSurface);
    if(FAILED(hr) || BaseSurface == NULL)
    {
        return hr;
    }
    
    D3D9Wrapper::IDirect3DSurface9* NewSurface = D3D9Wrapper::IDirect3DSurface9::GetSurface(BaseSurface);

    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "GetDepthStencilSurface called. D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(NewSurface)) << endl;
    }

    if(NewSurface == NULL)
    {
        BaseSurface->Release();
        return E_OUTOFMEMORY;
    }
    *ppZStencilSurface = NewSurface;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::BeginScene(THIS)
{
	ReportBeginScene();
    return m_pDevice->BeginScene();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::EndScene(THIS)
{
    ReportEndScene();
    return m_pDevice->EndScene();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::Clear(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	ReportClear(Count, pRects, Flags, Color, Z, Stencil);
	return m_pDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetTransform(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	HRESULT hr = m_pDevice->SetTransform(State, pMatrix);
	ReportSetTransform(State, pMatrix);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetTransform(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
	return m_pDevice->GetTransform(State, pMatrix);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::MultiplyTransform(THIS_ D3DTRANSFORMSTATETYPE a,CONST D3DMATRIX *b)
{
	g_Globals.UnreportedFile() << "MultiplyTransform\n";
	return m_pDevice->MultiplyTransform(a, b);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetViewport(THIS_ CONST D3DVIEWPORT9* pViewport)
{
    if(!SkipSetViewport)
    {
	    ReportSetViewport(pViewport);
    }
	return m_pDevice->SetViewport(pViewport);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetViewport(THIS_ D3DVIEWPORT9* pViewport)
{
	return m_pDevice->GetViewport(pViewport);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetMaterial(THIS_ CONST D3DMATERIAL9* pMaterial)
{
	ReportSetMaterial(pMaterial);
	return m_pDevice->SetMaterial(pMaterial);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetMaterial(THIS_ D3DMATERIAL9* pMaterial)
{
	return m_pDevice->GetMaterial(pMaterial);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetLight(THIS_ DWORD Index,CONST D3DLIGHT9 *Light)
{
	ReportSetLight(Index, Light);
	return m_pDevice->SetLight(Index, Light);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetLight(THIS_ DWORD Index,D3DLIGHT9* Light)
{
	return m_pDevice->GetLight(Index, Light);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::LightEnable(THIS_ DWORD Index,BOOL Enable)
{
	ReportLightEnable(Index, Enable);
	return m_pDevice->LightEnable(Index, Enable);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetLightEnable(THIS_ DWORD Index,BOOL* pEnable)
{
	return m_pDevice->GetLightEnable(Index, pEnable);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetClipPlane(THIS_ DWORD Index,CONST float* pPlane)
{
	return m_pDevice->SetClipPlane(Index, pPlane);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetClipPlane(THIS_ DWORD Index,float* pPlane)
{
	return m_pDevice->GetClipPlane(Index, pPlane);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetRenderState(THIS_ D3DRENDERSTATETYPE State,DWORD Value)
{
    if(!SkipSetRenderState)
    {
	    ReportSetRenderState(State, Value);
    }
	return m_pDevice->SetRenderState(State, Value);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetRenderState(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue)
{
	return m_pDevice->GetRenderState(State, pValue);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateStateBlock(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
	g_Globals.UnreportedFile() << "CreateStateBlock\n";
	return m_pDevice->CreateStateBlock(Type, ppSB);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::BeginStateBlock(THIS)
{
	g_Globals.UnreportedFile() << "BeginStateBlock\n";
	return m_pDevice->BeginStateBlock();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::EndStateBlock(THIS_ IDirect3DStateBlock9** ppSB)
{
	return m_pDevice->EndStateBlock(ppSB);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetClipStatus(THIS_ CONST D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDevice->SetClipStatus(pClipStatus);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetClipStatus(THIS_ D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDevice->GetClipStatus(pClipStatus);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetTexture(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
	*ppTexture = NULL;
	D3D9Base::LPDIRECT3DBASETEXTURE9 BaseTexture = NULL;
    HRESULT hr = m_pDevice->GetTexture(Stage, &BaseTexture);
    if(FAILED(hr) || BaseTexture == NULL)
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DBaseTexture9* NewTexture = D3D9Wrapper::IDirect3DBaseTexture9::GetBaseTexture(BaseTexture);
    if(NewTexture == NULL)
    {
        BaseTexture->Release();
        return E_OUTOFMEMORY;
    }
    *ppTexture = NewTexture;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetTexture(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
	if(pTexture == NULL)
    {
        D3D9Base::LPDIRECT3DSURFACE9 Surfaces[1];
        Surfaces[0] = NULL;
        ReportSetTexture(Stage, (HANDLE *)Surfaces, 1);
    }
    else
    {
        D3DRESOURCETYPE Type = pTexture->GetType();
        switch( Type )
        {
		case D3D9Base::D3DRTYPE_TEXTURE:
            {
				D3D9Base::IDirect3DTexture9* BaseTexture = ((D3D9Wrapper::IDirect3DTexture9*)pTexture)->GetTexture9();

                /*D3D9Base::LPDIRECT3DSURFACE9 TopLevelSurface[1];
                BaseTexture->GetSurfaceLevel(0, &TopLevelSurface[0]);
                ReportSetTexture(Stage, (HANDLE *)TopLevelSurface, 1);

                D3D9Base::LPDIRECT3DSURFACE9 TopLevelSurfaceTest;
		        BaseTexture->GetSurfaceLevel(0, &TopLevelSurfaceTest);

                g_Globals.ErrorFile() << "SetTexture: pTexture=" << pTexture << " BaseTexture=" << BaseTexture << " TopLevelSurface=" << TopLevelSurfaceTest << endl;

                TopLevelSurface[0]->Release();*/
				
                D3D9Base::LPDIRECT3DSURFACE9 Surfaces[1];
				Surfaces[0] = (D3D9Base::LPDIRECT3DSURFACE9)BaseTexture;
                ReportSetTexture(Stage, (HANDLE *)Surfaces, 1);

                //g_Globals.ErrorFile() << "SetTexture: pTexture=" << pTexture << " BaseTexture=" << BaseTexture << endl;
            }
            break;

		case D3D9Base::D3DRTYPE_VOLUMETEXTURE:
            g_Globals.UnreportedFile() << "Volume Set Texture\n";
            break;

        case D3D9Base::D3DRTYPE_CUBETEXTURE:
            //g_Globals.UnreportedFile() << "Cube Set Texture\n";
            break;
        }
    }

	if(pTexture && pTexture->GetType() == D3D9Base::D3DRTYPE_TEXTURE)
	{
		return m_pDevice->SetTexture(Stage, pTexture == 0 ? 0 : pTexture->GetBaseTexture9());
	}
	else
	{
		return m_pDevice->SetTexture(Stage, (D3D9Base::IDirect3DBaseTexture9*)pTexture);
	}
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetTextureStageState(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
	return m_pDevice->GetTextureStageState(Stage, Type, pValue);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetTextureStageState(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	ReportSetTextureStageState(Stage, Type, Value);

	return m_pDevice->SetTextureStageState(Stage, Type, Value);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetSamplerState(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{
	return m_pDevice->GetSamplerState(Sampler, Type, pValue);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetSamplerState(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	return m_pDevice->SetSamplerState(Sampler, Type, Value);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::ValidateDevice(THIS_ DWORD* pNumPasses)
{
	return m_pDevice->ValidateDevice(pNumPasses);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetPaletteEntries(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{
	return m_pDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetPaletteEntries(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries)
{
	return m_pDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetCurrentTexturePalette(THIS_ UINT PaletteNumber)
{
	return m_pDevice->SetCurrentTexturePalette(PaletteNumber);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetCurrentTexturePalette(THIS_ UINT *PaletteNumber)
{
	return m_pDevice->GetCurrentTexturePalette( PaletteNumber);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetScissorRect(THIS_ CONST RECT* pRect)
{
	return m_pDevice->SetScissorRect(pRect);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetScissorRect(THIS_ RECT* pRect)
{
	return m_pDevice->GetScissorRect(pRect);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetSoftwareVertexProcessing(THIS_ BOOL bSoftware)
{
	return m_pDevice->SetSoftwareVertexProcessing(bSoftware);
}

STDMETHODIMP_(BOOL) D3D9Wrapper::IDirect3DDevice9::GetSoftwareVertexProcessing(THIS)
{
	return m_pDevice->GetSoftwareVertexProcessing();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetNPatchMode(THIS_ float nSegments)
{
	return m_pDevice->SetNPatchMode(nSegments);
}

STDMETHODIMP_(float) D3D9Wrapper::IDirect3DDevice9::GetNPatchMode(THIS)
{
	return m_pDevice->GetNPatchMode();
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::DrawPrimitive(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	HRESULT hr = S_OK;
    if(g_Globals.UsingNullPixelShader)
    {
        m_Overlay.SetNullPixelShader();
    }
    hr = m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    if(ReportDrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount))
    {
        if(g_Globals.UsingNullPixelShader)
        {
            m_Overlay.SetNullPixelShader();
            m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
        }
    }
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::DrawIndexedPrimitive(THIS_ D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
    if(PreRenderQuery(Type, primCount))
    {
        return S_OK;
    }

	HRESULT hr = S_OK;
    if(g_Globals.UsingNullPixelShader)
    {
        m_Overlay.SetNullPixelShader();
    }
    hr = m_pDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
    if(ReportDrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount))
    {
        if(g_Globals.UsingNullPixelShader)
        {
            m_Overlay.SetNullPixelShader();
            m_pDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
        }
    }
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::DrawPrimitiveUP(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	HRESULT hr = S_OK;
    if(g_Globals.UsingNullPixelShader)
    {
        m_Overlay.SetNullPixelShader();
    }
    hr = m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    if(ReportDrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride))
    {
        if(g_Globals.UsingNullPixelShader)
        {
            m_Overlay.SetNullPixelShader();
            m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
        }
    }
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::DrawIndexedPrimitiveUP(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	HRESULT hr = S_OK;
    if(g_Globals.UsingNullPixelShader)
    {
        m_Overlay.SetNullPixelShader();
    }
    hr = m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    if(ReportDrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride))
    {
        if(g_Globals.UsingNullPixelShader)
        {
            m_Overlay.SetNullPixelShader();
            m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
        }
    }
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::ProcessVertices(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
	return m_pDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer->GetVB9(), pVertexDecl, Flags);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateVertexDeclaration(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
	//g_Globals.UnreportedFile() << "CreateVertexDeclaration\n";
	return m_pDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9* pDecl)
{
    if(!SkipSetVertexDeclaration)
    {
	    if(pDecl != NULL)
	    {
		    D3DVERTEXELEMENT9 Elements[64];
		    UINT ElementCount = 64;
		    HRESULT hr = pDecl->GetDeclaration(Elements, &ElementCount);
		    if(FAILED(hr))
		    {
			    g_Globals.ErrorFile() << "GetDeclaration failed\n";
		    }
		    else if(ElementCount > 0)
		    {
			    ReportSetVertexDeclaration(Elements, ElementCount);
		    }
	    }
    }
	return m_pDevice->SetVertexDeclaration(pDecl);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pDevice->GetVertexDeclaration(ppDecl);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetFVF(THIS_ DWORD FVF)
{
	ReportSetFVF(FVF);
	return m_pDevice->SetFVF(FVF);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetFVF(THIS_ DWORD* pFVF)
{
	return m_pDevice->GetFVF(pFVF);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateVertexShader(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	HRESULT hr = m_pDevice->CreateVertexShader(pFunction, ppShader);
	if(!FAILED(hr))
	{
		ReportCreateVertexShader(pFunction, *ppShader);
	}
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetVertexShader(THIS_ IDirect3DVertexShader9* pShader)
{
	ReportSetVertexShader(pShader);
	return m_pDevice->SetVertexShader(pShader);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetVertexShader(THIS_ IDirect3DVertexShader9** ppShader)
{
	return m_pDevice->GetVertexShader(ppShader);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetVertexShaderConstantF(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
    if(!SkipVertexBufferConstants && !g_Globals.VideoCaptureMode)
    {
	    ReportSetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }
	return m_pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetVertexShaderConstantF(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	return m_pDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetVertexShaderConstantI(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	g_Globals.UnreportedFile() << "SetVertexShaderConstantI\n";
	return m_pDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetVertexShaderConstantI(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetVertexShaderConstantB(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	g_Globals.UnreportedFile() << "SetVertexShaderConstantB\n";
	return m_pDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetVertexShaderConstantB(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetStreamSource(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
	if(pStreamData != NULL)
	{
        if(!SkipSetStreamSource)
        {
		    ReportSetStreamSource(StreamNumber, pStreamData->GetVB9(), OffsetInBytes, Stride);
        }
		return m_pDevice->SetStreamSource(StreamNumber, pStreamData->GetVB9(), OffsetInBytes, Stride);
	}
	else
	{
		return m_pDevice->SetStreamSource(StreamNumber, NULL, OffsetInBytes, Stride);
	}
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetStreamSource(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)
{
	g_Globals.ErrorFile() << "GetStreamSource called, duplicate vertex array.\n";
	*ppStreamData = NULL;
	D3D9Base::LPDIRECT3DVERTEXBUFFER9 BaseVB = NULL;
    HRESULT hr = m_pDevice->GetStreamSource(StreamNumber, &BaseVB, pOffsetInBytes, pStride);
    if( FAILED(hr) || BaseVB == NULL )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DVertexBuffer9* NewVB = D3D9Wrapper::IDirect3DVertexBuffer9::GetVertexBuffer(BaseVB);
    if( NewVB == NULL )
    {
        BaseVB->Release();
        return E_OUTOFMEMORY;
    }
    *ppStreamData = NewVB;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetStreamSourceFreq(THIS_ UINT StreamNumber, UINT FrequencyParameter)
{
    //g_Globals.UnreportedFile() << "SetStreamSourceFreq: " << StreamNumber << ", " << Setting << endl;
    ReportSetStreamSourceFreq(StreamNumber, FrequencyParameter);
	return m_pDevice->SetStreamSourceFreq(StreamNumber, FrequencyParameter);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetStreamSourceFreq(THIS_ UINT StreamNumber,UINT* pSetting)
{
	return m_pDevice->GetStreamSourceFreq(StreamNumber, pSetting);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetIndices(THIS_ IDirect3DIndexBuffer9* pIndexData)
{
    if(!SkipSetIndices)
    {
	    ReportSetIndices(NULL == pIndexData ? NULL : pIndexData->GetIB9());
    }
	return m_pDevice->SetIndices(NULL == pIndexData ? NULL : pIndexData->GetIB9());
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetIndices(THIS_ IDirect3DIndexBuffer9** ppIndexData)
{
	g_Globals.ErrorFile() << "GetIndices called, duplicate index array.\n";
	*ppIndexData = NULL;
	D3D9Base::LPDIRECT3DINDEXBUFFER9 BaseIB = NULL;
    HRESULT hr = m_pDevice->GetIndices(&BaseIB);
    if( FAILED(hr) || BaseIB == NULL )
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DIndexBuffer9* NewIB = D3D9Wrapper::IDirect3DIndexBuffer9::GetIndexBuffer(BaseIB);
    if( NewIB == NULL )
    {
        BaseIB->Release();
        return E_OUTOFMEMORY;
    }
    *ppIndexData = NewIB;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreatePixelShader(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
	HRESULT hr = m_pDevice->CreatePixelShader(pFunction, ppShader);
	if(!FAILED(hr))
	{
		ReportCreatePixelShader(pFunction, *ppShader);
	}
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetPixelShader(THIS_ IDirect3DPixelShader9* pShader)
{
	ReportSetPixelShader(pShader);
	return m_pDevice->SetPixelShader(pShader);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetPixelShader(THIS_ IDirect3DPixelShader9** ppShader)
{
	return m_pDevice->GetPixelShader(ppShader);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetPixelShaderConstantF(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
    ReportSetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	return m_pDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetPixelShaderConstantF(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	return m_pDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetPixelShaderConstantI(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
    g_Globals.UnreportedFile() << "SetPixelShaderConstantI\n";
	return m_pDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetPixelShaderConstantI(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::SetPixelShaderConstantB(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
    ReportSetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
	return m_pDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::GetPixelShaderConstantB(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::DrawRectPatch(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	g_Globals.UnreportedFile() << "DrawRectPatch\n";
	return m_pDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::DrawTriPatch(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	g_Globals.UnreportedFile() << "DrawTriPatch\n";
	return m_pDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::DeletePatch(THIS_ UINT Handle)
{
	return m_pDevice->DeletePatch(Handle);
}

STDMETHODIMP D3D9Wrapper::IDirect3DDevice9::CreateQuery(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
	//g_Globals.UnreportedFile() << "CreateQuery\n";
	HRESULT hr = m_pDevice->CreateQuery(Type, ppQuery);
	return hr;
}
