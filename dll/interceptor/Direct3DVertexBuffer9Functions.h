D3D9Wrapper::IDirect3DVertexBuffer9::IDirect3DVertexBuffer9( D3D9Base::LPDIRECT3DVERTEXBUFFER9 pVertexBuffer )
    : IDirect3DResource9((D3D9Base::LPDIRECT3DRESOURCE9) pVertexBuffer)
{
    m_pVertexBuffer = pVertexBuffer;
	Locked = false;
}

D3D9Wrapper::IDirect3DVertexBuffer9* D3D9Wrapper::IDirect3DVertexBuffer9::GetVertexBuffer(D3D9Base::LPDIRECT3DVERTEXBUFFER9 pVertexBuffer)
{
	return new D3D9Wrapper::IDirect3DVertexBuffer9(pVertexBuffer);
}

STDMETHODIMP D3D9Wrapper::IDirect3DVertexBuffer9::Lock(THIS_ UINT OffsetToLock,UINT SizeToLock,void** ppbData,DWORD Flags)
{
	if(IgnoringBufferLocks || g_Globals.VideoCaptureMode)
	{
		return m_pVertexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
	}
	if(Locked)
	{
		g_Globals.ErrorFile() << "Double lock\n";
	}
	HRESULT hr = m_pVertexBuffer->Lock(OffsetToLock, SizeToLock, &m_LockData.pVideoBuffer, Flags);
	if(FAILED(hr))
	{
		return hr;
	}
	Locked = true;
	m_LockData.Handle = m_pVertexBuffer;
	m_LockData.Flags = Flags;
	m_LockData.OffsetToLock = OffsetToLock;
	m_LockData.SizeToLock = SizeToLock;
	m_LockData.pRAMBuffer = NULL;
	m_LockData.Create = false;
	D3DVERTEXBUFFER_DESC Desc;
	m_pVertexBuffer->GetDesc(&Desc);
	if(m_LockData.SizeToLock == 0 && m_LockData.OffsetToLock == 0)
	{
		m_LockData.SizeToLock = Desc.Size;
	}
	ReportLockVertexBuffer(m_LockData, Desc);
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

STDMETHODIMP D3D9Wrapper::IDirect3DVertexBuffer9::Unlock(THIS)
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
	return m_pVertexBuffer->Unlock();
}

STDMETHODIMP D3D9Wrapper::IDirect3DVertexBuffer9::GetDesc(THIS_ D3DVERTEXBUFFER_DESC *pDesc)
{
	return m_pVertexBuffer->GetDesc(pDesc);
}

