D3D9Wrapper::IDirect3DBaseTexture9::IDirect3DBaseTexture9( D3D9Base::LPDIRECT3DBASETEXTURE9 pBaseTexture )
: IDirect3DResource9((D3D9Base::LPDIRECT3DRESOURCE9) pBaseTexture)
{
    m_pBaseTexture = pBaseTexture;
}

D3D9Wrapper::IDirect3DBaseTexture9* D3D9Wrapper::IDirect3DBaseTexture9::GetBaseTexture(D3D9Base::LPDIRECT3DBASETEXTURE9 pBaseTexture)
{
    D3DRESOURCETYPE Type = pBaseTexture->GetType();

    switch(Type)
    {
	case D3D9Base::D3DRTYPE_TEXTURE:
		return (D3D9Wrapper::IDirect3DBaseTexture9*) D3D9Wrapper::IDirect3DTexture9::GetTexture((D3D9Base::LPDIRECT3DTEXTURE9)pBaseTexture);
    /*case D3DRTYPE_VOLUMETEXTURE:
            return (D3D9Wrapper::IDirect3DBaseTexture9*) IDirect3DVolumeTexture8::GetVolumeTexture((LPDIRECT3DVOLUMETEXTURE9)pBaseTexture);
    case D3DRTYPE_CUBETEXTURE:
            return (D3D9Wrapper::IDirect3DBaseTexture9*) IDirect3DCubeTexture8::GetCubeTexture((LPDIRECT3DCUBETEXTURE9)pBaseTexture);*/
    }
    return NULL;
}

STDMETHODIMP_(DWORD) D3D9Wrapper::IDirect3DBaseTexture9::SetLOD(THIS_ DWORD LODNew)
{
	return m_pBaseTexture->SetLOD(LODNew);
}

STDMETHODIMP_(DWORD) D3D9Wrapper::IDirect3DBaseTexture9::GetLOD(THIS)
{
	return m_pBaseTexture->GetLOD();
}

STDMETHODIMP_(DWORD) D3D9Wrapper::IDirect3DBaseTexture9::GetLevelCount(THIS)
{
	return m_pBaseTexture->GetLevelCount();
}

STDMETHODIMP D3D9Wrapper::IDirect3DBaseTexture9::SetAutoGenFilterType(THIS_ D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pBaseTexture->SetAutoGenFilterType(FilterType);
}

STDMETHODIMP_(D3DTEXTUREFILTERTYPE) D3D9Wrapper::IDirect3DBaseTexture9::GetAutoGenFilterType(THIS)
{
	return m_pBaseTexture->GetAutoGenFilterType();
}

STDMETHODIMP_(void) D3D9Wrapper::IDirect3DBaseTexture9::GenerateMipSubLevels(THIS)
{
	return m_pBaseTexture->GenerateMipSubLevels();
}

