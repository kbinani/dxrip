struct Globals
{
	Globals()
	{
		Initialized = false;
        _InfoFile = NULL;
        _UnreportedFile = NULL;
        _ErrorFile = NULL;
	}
	void Init();

    bool Initialized;

	String BaseDirectory;
	String OutputFileDirectory;
	String RealD3D9DLL;
    bool UsingOverlay;
	UINT ConsoleLineCount;
    
    bool IgnoreTextureLocks;
    bool ForceSoftwareVertexProcessing;
    bool UsingNullPixelShader;
    bool VideoCaptureMode;
	
    __forceinline ofstream& InfoFile()
    {
        return *_InfoFile;
    }
    __forceinline ofstream& UnreportedFile()
    {
        return *_UnreportedFile;
    }
    __forceinline ofstream& ErrorFile()
    {
        return *_ErrorFile;
    }

private:
	ofstream *_InfoFile;
	ofstream *_UnreportedFile;
	ofstream *_ErrorFile;
};
