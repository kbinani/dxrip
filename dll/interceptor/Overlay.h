/*
Console.h
Written by Matthew Fisher

A simple console interface that can be used by the callback DLL to write feedback text to the user.
*/

struct ConsoleLine
{
	String Text;
	RGBColor Color;
};

struct ConsolePanel
{
    UINT CurHeadPtr;
	Vector<ConsoleLine> Lines;
};

class Overlay : public ID3D9DeviceOverlay
{
public:
	Overlay();
	void FreeMemory();
	void Init(D3D9Base::LPDIRECT3DDEVICE9 Device);
    void CreateNullPixelShader();
    void SetNullPixelShader();
	void OnLostDevice();
	void OnResetDevice();
    
    void WriteLine(const String &Text, RGBColor Color, UINT PanelIndex);
    void ClearPanel(UINT PanelIndex);

    void AddMesh(const Mesh &M);
    void SetMeshTransform(const Matrix4 &Transform);
    void ClearMeshes();

    void RenderConsole();
    void RenderMeshes();

private:
    static const UINT ConsolePanelCount = 5;
	ConsolePanel _Panels[ConsolePanelCount];

	D3D9Base::LPD3DXFONT _Font;
	D3D9Base::ID3DXSprite* _Sprite;
    D3D9Base::LPDIRECT3DDEVICE9 _Device;

    D3D9Base::LPDIRECT3DPIXELSHADER9 _NullPixelShader;

    Vector<Mesh *> _Meshes;
    Matrix4 _MeshTransform;
};
