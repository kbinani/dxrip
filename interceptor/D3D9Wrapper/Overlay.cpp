/*
Config.cpp
Written by Matthew Fisher

Implementation file for the Console class.
*/

#include "Main.h"

Overlay::Overlay()
{
	_Font = NULL;
    _NullPixelShader = NULL;
	_Sprite = NULL;
}

void Overlay::FreeMemory()
{
	if(_Font)
	{
		_Font->Release();
		_Font = NULL;
	}
	if(_Sprite)
	{
		_Sprite->Release();
		_Sprite = NULL;
	}
    if(_NullPixelShader)
    {
        _NullPixelShader->Release();
        _NullPixelShader = NULL;
    }
}

void Overlay::OnLostDevice()
{
	if(_Font)
	{
		_Font->OnLostDevice();
	}
	if(_Sprite)
	{
		_Sprite->OnLostDevice();
	}
    if(_NullPixelShader)
    {
        _NullPixelShader->Release();
        _NullPixelShader = NULL;
    }
}

void Overlay::OnResetDevice()
{
	if(_Font)
	{
		_Font->OnResetDevice();
	}
	if(_Sprite)
	{
		_Sprite->OnResetDevice();
	}
    if(_NullPixelShader)
    {
        _NullPixelShader->Release();
        _NullPixelShader = NULL;
    }
}

void Overlay::ClearPanel(UINT PanelIndex)
{
    if(PanelIndex >= ConsolePanelCount)
    {
        return;
    }

    if(g_Globals.UsingOverlay)
	{
        ConsolePanel &CurPanel = _Panels[PanelIndex];
        CurPanel.CurHeadPtr = 0;
		CurPanel.Lines.Allocate(g_Globals.ConsoleLineCount);
    }
}

void Overlay::Init(D3D9Base::LPDIRECT3DDEVICE9 Device)
{
	if(g_Globals.UsingOverlay)
	{
        _Device = Device;
		FreeMemory();
		HDC hDC = GetDC( NULL );
		int nLogPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
		ReleaseDC( NULL, hDC );
		int FontHeight = -9 * nLogPixelsY / 72;	//get the appropriate height of the font
		HRESULT hr = D3DXCreateFont( Device,
						FontHeight,
						0,
						FW_BOLD,
						1,
						FALSE,
						DEFAULT_CHARSET,
						OUT_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_DONTCARE,
						"Arial",
						&_Font);
		Assert(SUCCEEDED(hr), "D3DXCreateFont failed");

        for(UINT PanelIndex = 0; PanelIndex < ConsolePanelCount; PanelIndex++)
        {
            ClearPanel(PanelIndex);
        }
		hr = D3DXCreateSprite(Device, &_Sprite);
		Assert(SUCCEEDED(hr), "D3DXCreateSprite failed");

        WriteLine("Console Start", RGBColor::Yellow, 0);
	}
}

void Overlay::CreateNullPixelShader()
{
    HRESULT hr;

    DWORD dwShaderFlags = 0;
    D3D9Base::LPD3DXBUFFER pCode = NULL;
    D3D9Base::LPD3DXBUFFER pErrors = NULL;

    String ShaderFilename = g_Globals.BaseDirectory + String("\\") + String("Null.psh");

    PersistentAssert(Utility::FileExists(ShaderFilename), "Null.psh not found");

    // Assemble the vertex shader from the file
    hr = D3DXCompileShaderFromFile( ShaderFilename.CString(), NULL, NULL, "PShaderEntry",
                                    "ps_3_0", dwShaderFlags, &pCode,
                                    &pErrors, NULL );
    
    String ErrorString;
    if(pErrors)
    {
        char *ErrorMessage = (char *)pErrors->GetBufferPointer();
        DWORD ErrorLength = pErrors->GetBufferSize();

        for(UINT i = 0; i < ErrorLength; i++)
        {
            g_Globals.ErrorFile() << ErrorMessage[i];
            ErrorString += String(ErrorMessage[i]);
        }
    }

    PersistentAssert(!FAILED(hr), String("D3DXCompileShaderFromFile failed: ") + ErrorString);

    // Create the pixel shader
    hr = _Device->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(),
                                     &_NullPixelShader );

    if(pErrors)
    {
        pErrors->Release();
    }
    if(pCode)
    {
        pCode->Release();
    }
    PersistentAssert(!FAILED(hr), "CreatePixelShader failed");
}

void Overlay::SetNullPixelShader()
{
    if(_NullPixelShader == NULL)
    {
        CreateNullPixelShader();
    }
    HRESULT hr = _Device->SetPixelShader(_NullPixelShader);
    Assert(SUCCEEDED(hr), "SetPixelShader failed");
}

void Overlay::WriteLine(const String &Text, RGBColor Color, UINT PanelIndex)
{
    if(PanelIndex >= ConsolePanelCount)
    {
        return;
    }

	if(g_Globals.UsingOverlay)
	{
        ConsolePanel &CurPanel = _Panels[PanelIndex];
		CurPanel.Lines[CurPanel.CurHeadPtr].Text = Text;
		CurPanel.Lines[CurPanel.CurHeadPtr].Color = Color;
		CurPanel.CurHeadPtr = Math::Mod(CurPanel.CurHeadPtr + 1, CurPanel.Lines.Length());
	}
}

void Overlay::AddMesh(const Mesh &M)
{
    _Meshes.PushEnd(new Mesh(M));
}

void Overlay::SetMeshTransform(const Matrix4 &Transform)
{
    _MeshTransform = Transform;
}

void Overlay::ClearMeshes()
{
    for(UINT MeshIndex = 0; MeshIndex < _Meshes.Length(); MeshIndex++)
    {
        delete _Meshes[MeshIndex];
    }
    _Meshes.FreeMemory();
}

bool Pressed(SHORT Key)
{
    return (Key == -32767 || Key == -32768);
}

D3D9Base::D3DXMATRIX Matrix4ToD3DXMATRIX(const Matrix4 &M)
{
    D3D9Base::D3DXMATRIX Result;
    for(UINT i = 0; i < 4; i++)
    {
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            Result(i, i2) = M[i][i2];
        }
    }
    return Result;
}

void Overlay::RenderMeshes()
{
    if(g_Globals.UsingOverlay && _Meshes.Length() > 0)
	{
        IDirect3DStateBlock9* pStateBlock = NULL;
        _Device->CreateStateBlock( D3D9Base::D3DSBT_ALL, &pStateBlock );

        const UINT MaxTextureSlots = 8;
        for(UINT TextureIndex = 0; TextureIndex < MaxTextureSlots; TextureIndex++)
        {
            _Device->SetTexture(TextureIndex, NULL);
        }

        D3D9Base::D3DXMATRIXA16 NewTransformWorld, NewTransformView, NewTransformProjection;
        NewTransformWorld = Matrix4ToD3DXMATRIX(Matrix4::Identity());
        NewTransformView = Matrix4ToD3DXMATRIX(Matrix4::Identity());
        NewTransformProjection = Matrix4ToD3DXMATRIX(_MeshTransform);

        _Device->SetTransform(D3DTS_WORLD, &NewTransformWorld);
        _Device->SetTransform(D3D9Base::D3DTS_VIEW, &NewTransformView);
        _Device->SetTransform(D3D9Base::D3DTS_PROJECTION, &NewTransformProjection);

        _Device->SetRenderState(D3D9Base::D3DRS_ALPHABLENDENABLE, FALSE);
        //_Device->SetRenderState(D3D9Base::D3DRS_ZENABLE, D3D9Base::D3DZB_TRUE);
        _Device->SetRenderState(D3D9Base::D3DRS_ZENABLE, D3D9Base::D3DZB_FALSE);
        _Device->SetRenderState(D3D9Base::D3DRS_LIGHTING, FALSE);
        _Device->SetRenderState(D3D9Base::D3DRS_ZFUNC, D3D9Base::D3DCMP_ALWAYS);
        _Device->SetRenderState(D3D9Base::D3DRS_ALPHATESTENABLE, FALSE);
        _Device->SetRenderState(D3D9Base::D3DRS_CULLMODE, D3D9Base::D3DCULL_NONE);

        const DWORD D3DMeshFlags = D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEX1;
        _Device->SetFVF(D3DMeshFlags);
        
        _Device->SetVertexShader(NULL);
        _Device->SetPixelShader(NULL);
        
        for(UINT MeshIndex = 0; MeshIndex < _Meshes.Length(); MeshIndex++)
        {
            Mesh &CurMesh = *(_Meshes[MeshIndex]);
            _Device->DrawIndexedPrimitiveUP(D3D9Base::D3DPT_TRIANGLELIST, 0, CurMesh.VertexCount(), CurMesh.FaceCount(), CurMesh.Indices(), D3D9Base::D3DFMT_INDEX32, CurMesh.Vertices(), sizeof(MeshVertex));
        }

        pStateBlock->Apply();
        pStateBlock->Release();
    }
}

void Overlay::RenderConsole()
{
    const UINT ConsoleStartOffset = 3;
    const UINT ConsoleXSeparation = 210;
	const UINT ConsoleYSeparation = 13;

    if(Pressed(GetAsyncKeyState(KEY_F2)))
    {
        g_Globals.UsingNullPixelShader = !g_Globals.UsingNullPixelShader;
    }

    if(g_Globals.UsingOverlay)
	{
        if(Pressed(GetAsyncKeyState(KEY_DELETE)))
        {
            for(UINT PanelIndex = 0; PanelIndex < ConsolePanelCount; PanelIndex++)
            {
                ClearPanel(PanelIndex);
            }
            ClearMeshes();
        }

        D3D9Base::LPDIRECT3DDEVICE9 Device = NULL;
        _Font->GetDevice(&Device);

        D3D9Base::LPDIRECT3DSURFACE9 BackBufferSurface = NULL;
        Device->GetBackBuffer(0, 0, D3D9Base::D3DBACKBUFFER_TYPE_MONO, &BackBufferSurface);
        
        D3DSURFACE_DESC BackBufferDesc;
        BackBufferSurface->GetDesc(&BackBufferDesc);

        D3DVIEWPORT9 Viewport;
        Viewport.X = 0;
        Viewport.Y = 0;
        Viewport.Width = BackBufferDesc.Width;
        Viewport.Height = BackBufferDesc.Height;
        Viewport.MinZ = 0.0f;
        Viewport.MaxZ = 1.0f;
        Device->SetViewport(&Viewport);

        BackBufferSurface->Release();
        Device->Release();

		_Sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

        for(UINT PanelIndex = 0; PanelIndex < ConsolePanelCount; PanelIndex++)
        {
            ConsolePanel &CurPanel = _Panels[PanelIndex];
		    UINT LineIndex = CurPanel.CurHeadPtr, YPos = ConsoleStartOffset;
		    UINT XPos = ConsoleStartOffset + ConsoleXSeparation * PanelIndex;
			bool FlipLineOrder = false;
			if(PanelIndex == ConsolePanelCount - 1)
			{
				FlipLineOrder = true;
				//YPos = ConsoleStartOffset + ConsoleYSeparation * CurPanel.Lines.Length();
				YPos = ConsoleStartOffset + ConsoleYSeparation;
				XPos = ConsoleStartOffset;
			}
			if(PanelIndex == ConsolePanelCount - 2)
			{
				XPos += 75;
			}

			bool Done = false;
		    while(!Done)
		    {
			    // TODO: add sprite support
				UINT ModifiedLineIndex = LineIndex;
				if(FlipLineOrder)
				{
					//ModifiedLineIndex = CurPanel.Lines.Length() - 1 - LineIndex;
				}
                const ConsoleLine &CurLine = CurPanel.Lines[ModifiedLineIndex];
                const String &CurText = CurLine.Text;
			    if(CurText.Length() > 0)
			    {
				    RECT Rect;
				    SetRect( &Rect, int(XPos), int(YPos), 0, 0 );
    				
                    //RGBColor Color = RGBColor::Black;
			        //_Font->DrawText(_Sprite, CurText.CString(), CurText.Length(), &Rect, DT_NOCLIP, D3D9Base::D3DXCOLOR(Color.r / 255.0f, Color.g / 255.0f, Color.b / 255.0f, 1.0f ));
			        //Rect.left -= 1;
			        //Rect.top -= 1;

                    RGBColor Color = CurLine.Color;
			        _Font->DrawText(_Sprite, CurText.CString(), CurText.Length(), &Rect, DT_NOCLIP, D3D9Base::D3DXCOLOR(Color.r / 255.0f, Color.g / 255.0f, Color.b / 255.0f, 1.0f ));
			    }
			    LineIndex = Math::Mod(LineIndex + 1, CurPanel.Lines.Length());
			    YPos += ConsoleYSeparation;
			    if(LineIndex == Math::Mod(int(CurPanel.CurHeadPtr), CurPanel.Lines.Length()))
			    {
				    Done = true;
			    }
		    }
        }
		_Sprite->End();
	}
}