
D3D9Wrapper::IDirect3DSurface9::IDirect3DSurface9( D3D9Base::LPDIRECT3DSURFACE9 pSurface )
    : IDirect3DResource9((D3D9Base::LPDIRECT3DRESOURCE9) pSurface)
{
    m_pSurface = pSurface;
}

D3D9Wrapper::IDirect3DSurface9* D3D9Wrapper::IDirect3DSurface9::GetSurface(D3D9Base::LPDIRECT3DSURFACE9 pSurface)
{
    D3D9Wrapper::IDirect3DSurface9* p = (D3D9Wrapper::IDirect3DSurface9*) m_List.GetDataPtr(pSurface);
    if(p == NULL)
    {
        p = new IDirect3DSurface9(pSurface);
        if(DebuggingSurfaceCreation)
        {
            g_Globals.InfoFile() << "GetSurface on D3D9Base " << String::UnsignedIntAsHex(UINT(pSurface)) << " created D3D9Wrapper " << String::UnsignedIntAsHex(UINT(p)) << endl;
        }
        m_List.AddMember(pSurface, p);
        return p;
    }
    
    p->m_ulRef++;
    return p;
}

STDMETHODIMP_(ULONG) D3D9Wrapper::IDirect3DSurface9::Release(THIS)
{
    m_pUnk->Release();

    ULONG ulRef = --m_ulRef;
    if(ulRef == 0)
    {
        m_List.DeleteMember(GetSurface9());
        delete this;
        return 0;
    }
    return ulRef;
}

STDMETHODIMP D3D9Wrapper::IDirect3DSurface9::GetContainer(THIS_ REFIID riid,void** ppContainer)
{
	return m_pSurface->GetContainer(riid, ppContainer);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSurface9::GetDesc(THIS_ D3DSURFACE_DESC *pDesc)
{
	return m_pSurface->GetDesc( pDesc);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSurface9::LockRect(THIS_ D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
    return m_pSurface->LockRect(pLockedRect, pRect, Flags);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSurface9::UnlockRect(THIS)
{
    HRESULT hr = m_pSurface->UnlockRect();

    //g_Globals.InfoFile() << "UnlockRect: " << this << " hr=" << hr << endl;
	if(!FAILED(hr))
	{
		D3DSURFACE_DESC MyDesc, TopLevelDesc;
	    hr = GetDesc(&MyDesc);
	    Assert(SUCCEEDED(hr), "SrcSurface->GetDesc failed");

        D3D9Base::LPDIRECT3DTEXTURE9 BaseTexture = NULL;
        hr = GetSurface9()->GetContainer(D3D9Base::IID_IDirect3DTexture9, (void **)&BaseTexture);
        //g_Globals.InfoFile() << "GetContainer: " << this << endl;
        
        if(SUCCEEDED(hr))
        {
            hr = BaseTexture->GetLevelDesc(0, &TopLevelDesc);
            Assert(SUCCEEDED(hr), "BaseTexture->GetLevelDesc failed");
            //g_Globals.InfoFile() << "TopLevel check: " << this << " TopLevel=" << TopLevelDesc.Width << "x" << TopLevelDesc.Height << " this=" << MyDesc.Width << "x" << MyDesc.Height << endl;
            if(TopLevelDesc.Width == MyDesc.Width && TopLevelDesc.Height == MyDesc.Height)
            {
                Bitmap Bmp;
                if(g_Globals.IgnoreTextureLocks)
                {
                    Bmp.Allocate(1, 1);
                    Bmp.Clear(RGBColor::Magenta);
                }
                else
                {
                    D3D9Wrapper::IDirect3DDevice9* Device;
		            GetDevice(&Device);

                    RECT Rect;
                    Rect.left = 0;
	                Rect.right = MyDesc.Width;
	                Rect.top = 0;
	                Rect.bottom = MyDesc.Height;
		            Device->BitmapFromSurface(m_pSurface, Rect, Bmp);
                    Device->Release();
                }

                //g_Globals.InfoFile() << "ReportUnlockTexture: " << this << endl;
                ReportUnlockTexture(MyDesc, Bmp, BaseTexture);
            }
            BaseTexture->Release();
        }
        else
        {
            //g_Globals.ErrorFile() << "GetContainer failed\n"; (could be a cubemap)
        }

        //g_Globals.ErrorFile() << "IDirect3DSurface9::UnlockRect BaseTexture=" << BaseTexture << endl;
	}

	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DSurface9::GetDC(THIS_ HDC *phdc)
{
	return m_pSurface->GetDC( phdc);
}

STDMETHODIMP D3D9Wrapper::IDirect3DSurface9::ReleaseDC(THIS_ HDC hdc)
{
	return m_pSurface->ReleaseDC(hdc);
}
