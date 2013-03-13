D3D9Wrapper::IDirect3DResource9::IDirect3DResource9( D3D9Base::LPDIRECT3DRESOURCE9 pResource )
    : IDirect3DUnknown((IUnknown*) pResource)
{
    m_pResource = pResource;
}

D3D9Wrapper::IDirect3DResource9* D3D9Wrapper::IDirect3DResource9::GetResource(D3D9Base::LPDIRECT3DRESOURCE9 pResource)
{
	D3DRESOURCETYPE Type = pResource->GetType();

	switch(Type)
	{
	case D3D9Base::D3DRTYPE_SURFACE:
        {
            D3D9Wrapper::IDirect3DResource9 *Result = (D3D9Wrapper::IDirect3DResource9*) D3D9Wrapper::IDirect3DSurface9::GetSurface((D3D9Base::LPDIRECT3DSURFACE9)pResource);
            if(DebuggingSurfaceCreation)
            {
                g_Globals.InfoFile() << "GetResource called on D3DRTYPE_SURFACE. D3D9Base: " << String::UnsignedIntAsHex(UINT(pResource)) << " D3D9Wrapper: " << String::UnsignedIntAsHex(UINT(Result)) << endl;
            }
		    return Result;
        }
	/*case D3DRTYPE_VOLUME:
		return (D3D9Wrapper::IDirect3DResource9*) D3D9Wrapper::IDirect3DVolume9::GetVolume((D3D9Base::LPDIRECT3DVOLUME9)pResource);*/
	case D3D9Base::D3DRTYPE_TEXTURE:
		return (D3D9Wrapper::IDirect3DResource9*) D3D9Wrapper::IDirect3DTexture9::GetTexture((D3D9Base::LPDIRECT3DTEXTURE9)pResource);
	/*case D3DRTYPE_VOLUMETEXTURE:
		return (D3D9Wrapper::IDirect3DResource9*) D3D9Wrapper::IDirect3DVolumeTexture9::GetVolumeTexture((D3D9Base::LPDIRECT3DVOLUMETEXTURE9)pResource);
	case D3DRTYPE_CUBETEXTURE:
		return (D3D9Wrapper::IDirect3DResource9*) D3D9Wrapper::IDirect3DCubeTexture9::GetCubeTexture((D3D9Base::LPDIRECT3DCUBETEXTURE9)pResource);
	case D3DRTYPE_VERTEXBUFFER:
		return (D3D9Wrapper::IDirect3DResource9*) D3D9Wrapper::IDirect3DVertexBuffer9::GetVertexBuffer((D3D9Base::LPDIRECT3DVERTEXBUFFER9)pResource);
	case D3DRTYPE_INDEXBUFFER:
		return (D3D9Wrapper::IDirect3DResource9*) D3D9Wrapper::IDirect3DIndexBuffer9::GetIndexBuffer((D3D9Base::LPDIRECT3DINDEXBUFFER9)pResource);*/
	}
	return NULL;
}

STDMETHODIMP D3D9Wrapper::IDirect3DResource9::GetDevice(THIS_ D3D9Wrapper::IDirect3DDevice9** ppDevice)
{
	D3D9Base::LPDIRECT3DDEVICE9 BaseDevice = NULL;
	*ppDevice = NULL;

    HRESULT hr = m_pResource->GetDevice(&BaseDevice);
    if(FAILED(hr))
    {
        return hr;
    }
    
	D3D9Wrapper::IDirect3DDevice9* NewDevice = IDirect3DDevice9::GetDirect3DDevice(BaseDevice);
    if(NewDevice == NULL)
    {
        BaseDevice->Release();
        return E_OUTOFMEMORY;
    }
    *ppDevice = NewDevice;
    return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DResource9::SetPrivateData(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)
{
	return m_pResource->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP D3D9Wrapper::IDirect3DResource9::GetPrivateData(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData)
{
	return m_pResource->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP D3D9Wrapper::IDirect3DResource9::FreePrivateData(THIS_ REFGUID refguid)
{
	return m_pResource->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) D3D9Wrapper::IDirect3DResource9::SetPriority(THIS_ DWORD PriorityNew)
{
	return m_pResource->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) D3D9Wrapper::IDirect3DResource9::GetPriority(THIS)
{
	return m_pResource->GetPriority();
}

STDMETHODIMP_(void) D3D9Wrapper::IDirect3DResource9::PreLoad(THIS)
{
	return m_pResource->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) D3D9Wrapper::IDirect3DResource9::GetType(THIS)
{
	return m_pResource->GetType();
}

