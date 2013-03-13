D3D9Wrapper::IDirect3DTexture9::IDirect3DTexture9( D3D9Base::LPDIRECT3DTEXTURE9 pTexture )
    : IDirect3DBaseTexture9((D3D9Base::LPDIRECT3DBASETEXTURE9) pTexture)
{
    m_pTexture = pTexture;
}

D3D9Wrapper::IDirect3DTexture9* D3D9Wrapper::IDirect3DTexture9::GetTexture(D3D9Base::LPDIRECT3DTEXTURE9 pTexture)
{
    return new D3D9Wrapper::IDirect3DTexture9(pTexture);
}

STDMETHODIMP_(ULONG) D3D9Wrapper::IDirect3DTexture9::Release(THIS)
{
	Assert(m_ulRef > 0, "Release on unreferenced object");
	DWORD SurfaceLevelCount = m_pTexture->GetLevelCount();
	D3D9Base::LPDIRECT3DSURFACE9 Surfaces[MaxSurfaceLevels];
    if(SurfaceLevelCount == 0 || SurfaceLevelCount > MaxSurfaceLevels)
    {
		g_Globals.ErrorFile() << "SurfaceLevelCount == 0 || SurfaceLevelCount > MaxSurfaceLevels: " << SurfaceLevelCount << endl;
    }
    else
    {
        for(UINT i = 0; i < SurfaceLevelCount; i++)
        {
            Surfaces[i] = NULL;
            m_pTexture->GetSurfaceLevel(i, &(Surfaces[i]));
            if(Surfaces[i])
            {
                Surfaces[i]->Release();
            }
        }
    }

	m_pUnk->Release();

    ULONG ulRef = --m_ulRef;
    if(ulRef == 0)
    {
		for(UINT i = 0; i < SurfaceLevelCount; i++)
        {
			ReportDestroy(Surfaces[i]);
        }
        delete this;
        return 0;
    }
    return ulRef;
}

STDMETHODIMP D3D9Wrapper::IDirect3DTexture9::GetLevelDesc(THIS_ UINT Level,D3DSURFACE_DESC *pDesc)
{
	return m_pTexture->GetLevelDesc(Level,  pDesc);
}

STDMETHODIMP D3D9Wrapper::IDirect3DTexture9::GetSurfaceLevel(THIS_ UINT Level,IDirect3DSurface9** ppSurfaceLevel)
{
	D3D9Base::LPDIRECT3DSURFACE9 BaseSurface = NULL;
	*ppSurfaceLevel = NULL;
    HRESULT hr = m_pTexture->GetSurfaceLevel(Level, &BaseSurface);
    if(FAILED(hr) || BaseSurface == NULL)
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DSurface9* NewSurface = D3D9Wrapper::IDirect3DSurface9::GetSurface(BaseSurface);

    if(DebuggingSurfaceCreation)
    {
        g_Globals.InfoFile() << "GetSurfaceLevel" << Level << " D3D9Base: " << String::UnsignedIntAsHex(UINT(BaseSurface)) << " created D3D9Wrapper " << String::UnsignedIntAsHex(UINT(NewSurface)) << endl;
    }

    if(NewSurface == NULL)
    {
        BaseSurface->Release();
        return E_OUTOFMEMORY;
    }
    *ppSurfaceLevel = NewSurface;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DTexture9::LockRect(THIS_ UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	return m_pTexture->LockRect(Level, pLockedRect, pRect, Flags);
}

STDMETHODIMP D3D9Wrapper::IDirect3DTexture9::UnlockRect(THIS_ UINT Level)
{
	HRESULT hr = m_pTexture->UnlockRect(Level);

	if(Level == 0 && !FAILED(hr))
	{
		D3D9Base::LPDIRECT3DSURFACE9 TopLevelSurface = NULL;
		D3D9Wrapper::IDirect3DDevice9* m_pDevice = NULL;
		hr = m_pTexture->GetSurfaceLevel(0, &TopLevelSurface);
        
        if(FAILED(hr) || TopLevelSurface == NULL)
		{
			g_Globals.ErrorFile() << "IDirect3DTexture9::UnlockRect GetSurfaceLevel failed\n";
			return hr;
		}

		GetDevice(&m_pDevice);
		Bitmap Bmp;
        if(g_Globals.IgnoreTextureLocks)
        {
		    Bmp.Allocate(1, 1);
            Bmp.Clear(RGBColor::Magenta);
        }
        else
        {
            D3DSURFACE_DESC Desc;
	        TopLevelSurface->GetDesc(&Desc);
            RECT Rect;
            Rect.left = 0;
	        Rect.right = Desc.Width;
	        Rect.top = 0;
	        Rect.bottom = Desc.Height;
            m_pDevice->BitmapFromSurface(TopLevelSurface, Rect, Bmp);
        }

        D3DSURFACE_DESC Desc;
		hr = TopLevelSurface->GetDesc(&Desc);
		Assert(SUCCEEDED(hr), "TopLevelSurface->GetDesc failed");

        //g_Globals.ErrorFile() << "IDirect3DTexture9::UnlockRect BaseTexture=" << m_pTexture << " TopLevelSurface=" << TopLevelSurface << endl;

		ReportUnlockTexture(Desc, Bmp, m_pTexture);
		TopLevelSurface->Release();
		m_pDevice->Release();

		//g_Globals.InfoFile() << "Unlock\t" << this << '\t' << m_pTexture << '\t' << SrcSurface << endl;

		/*LD.Handle = SrcSurface;
		
		SrcSurface->GetDesc(&LD.Desc);
		SrcSurface->GetDevice(&m_pDevice);
		DestSurface = D3D9Wrapper::IDirect3DDevice9::GetDirect3DDevice(m_pDevice)->GetOffscreenSurface();
		RECT Rect;
		Rect.left = 0;
		Rect.right = LD.Desc.Width;
		Rect.top = 0;
		Rect.bottom = LD.Desc.Height;
#ifdef USE_D3DX
		D3D9Base::D3DXLoadSurfaceFromSurface(DestSurface, NULL, &Rect, SrcSurface, NULL, &Rect, D3DX_FILTER_POINT, D3DCOLOR_XRGB(0, 0, 0));
		DestSurface->LockRect(&LD.LockedRect, &Rect, NULL);
		UnlockReport(LD);
		DestSurface->UnlockRect();
#endif*/
	}

	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DTexture9::AddDirtyRect(THIS_ CONST RECT* pDirtyRect)
{
	return m_pTexture->AddDirtyRect(pDirtyRect);
}
