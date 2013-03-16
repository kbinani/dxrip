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
    bool WritingToFiles;

	String BaseDirectory;
	String OutputFileDirectory;
	String RealD3D9DLL;
    bool UsingOverlay;
	UINT ConsoleLineCount;
    
    bool IgnoreTextureLocks;
    bool ForceSoftwareVertexProcessing;
    bool UsingNullPixelShader;
    bool VideoCaptureMode;

    __forceinline ostream &InfoFile()
    {
        return *_InfoFile;
    }
    __forceinline ostream &UnreportedFile()
    {
        return *_UnreportedFile;
    }
    __forceinline ostream &ErrorFile()
    {
        return *_ErrorFile;
    }

private:
    ostream *_InfoFile;
    ostream *_UnreportedFile;
    ostream *_ErrorFile;
};
