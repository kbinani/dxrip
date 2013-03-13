D3D9Wrapper::IDirect3DIndexBuffer9::IDirect3DIndexBuffer9( D3D9Base::LPDIRECT3DINDEXBUFFER9 pIndexBuffer )
    : IDirect3DResource9((D3D9Base::LPDIRECT3DRESOURCE9) pIndexBuffer)
{
    m_pIndexBuffer = pIndexBuffer;
	Locked = false;
}

D3D9Wrapper::IDirect3DIndexBuffer9* D3D9Wrapper::IDirect3DIndexBuffer9::GetIndexBuffer(D3D9Base::LPDIRECT3DINDEXBUFFER9 pIndexBuffer)
{
	return new D3D9Wrapper::IDirect3DIndexBuffer9(pIndexBuffer);
}

STDMETHODIMP D3D9Wrapper::IDirect3DIndexBuffer9::Lock(THIS_ UINT OffsetToLock,UINT SizeToLock,void** ppbData,DWORD Flags)
{
    if(IgnoringBufferLocks || g_Globals.VideoCaptureMode)
	{
		return m_pIndexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
	}
	if(Locked)
	{
		g_Globals.ErrorFile() << "Double lock\n";
		//return E_OUTOFMEMORY;
	}
	HRESULT hr = m_pIndexBuffer->Lock(OffsetToLock, SizeToLock, &m_LockData.pVideoBuffer, Flags);
	if(FAILED(hr))
	{
		return hr;
	}
	Locked = true;
	m_LockData.Handle = m_pIndexBuffer;
	m_LockData.Flags = Flags;
	m_LockData.OffsetToLock = OffsetToLock;
	m_LockData.SizeToLock = SizeToLock;
	m_LockData.pRAMBuffer = NULL;
	m_LockData.Create = false;
	D3DINDEXBUFFER_DESC Desc;
	m_pIndexBuffer->GetDesc(&Desc);
	if(m_LockData.SizeToLock == 0 && m_LockData.OffsetToLock == 0)
	{
		m_LockData.SizeToLock = Desc.Size;
	}
	ReportLockIndexBuffer(m_LockData, Desc);
	if(m_LockData.pRAMBuffer == NULL)
	{
		g_Globals.ErrorFile() << "Lock, m_LockData.pRAMBuffer == NULL\n";
		*ppbData = m_LockData.pVideoBuffer;
	}
	else
	{
		*ppbData = m_LockData.pRAMBuffer;
	}
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DIndexBuffer9::Unlock(THIS)
{
	if(!IgnoringBufferLocks && !g_Globals.VideoCaptureMode)
	{
		Locked = false;
		if(m_LockData.pRAMBuffer == NULL)
		{
			g_Globals.ErrorFile() << "Unlock, m_LockData.pRAMBuffer == NULL\n";
		}
		else
		{
			memcpy(m_LockData.pVideoBuffer, m_LockData.pRAMBuffer, m_LockData.SizeToLock);
		}
	}
	return m_pIndexBuffer->Unlock();
}

STDMETHODIMP D3D9Wrapper::IDirect3DIndexBuffer9::GetDesc(THIS_ D3DINDEXBUFFER_DESC *pDesc)
{
	return m_pIndexBuffer->GetDesc( pDesc);
}

