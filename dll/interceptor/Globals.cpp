#include "Main.h"
#include "nullstream.h"

void Globals::Init()
{
	Initialized = true;
	
    UsingNullPixelShader = false;
    String fileName = String("WrapperParameters.txt");
    if (Utility::FileExists(fileName)) {
	    ParameterFile Parameters(fileName);
	    BaseDirectory = Parameters.GetRequiredString("BaseDirectory");
	    OutputFileDirectory = BaseDirectory + String("\\") + Parameters.GetRequiredString("OutputFileDirectory") + String("\\");
	    RealD3D9DLL = Parameters.GetRequiredString("RealD3D9DLL");
	
	    UsingOverlay = Parameters.GetBoolean("UsingOverlay");
	    ConsoleLineCount = Parameters.GetUnsignedInteger("ConsoleLineCount");

        ForceSoftwareVertexProcessing = Parameters.GetBoolean("ForceSoftwareVertexProcessing");
        IgnoreTextureLocks = Parameters.GetBoolean("IgnoreTextureLocks");

        VideoCaptureMode = Parameters.GetBoolean("VideoCaptureMode");
        WritingToFiles = true;
    } else {
        char systemDirectory[_MAX_PATH + 1] = "";
        ::GetSystemDirectoryA(systemDirectory, _MAX_PATH);
        RealD3D9DLL = String(systemDirectory) + String("\\d3d9.dll");
        UsingOverlay = false;
        IgnoreTextureLocks = true;
        VideoCaptureMode = false;
        WritingToFiles = false;
    }
    if(VideoCaptureMode)
    {
        IgnoreTextureLocks = true;
    }

    if (WritingToFiles) {
        const char *infoFilePath = (OutputFileDirectory + String("WrapperInfo.txt")).CString();
        _InfoFile = new ofstream(infoFilePath);
        PersistentAssert(!_InfoFile->fail(), "Failed to open InfoFile");

        const char *unreportedFilePath = (OutputFileDirectory + String("WrapperUnreportedEvents.txt")).CString();
        _UnreportedFile = new ofstream(unreportedFilePath);
        PersistentAssert(!_UnreportedFile->fail(), "Failed to open UnreportedFile");

        const char *errorFilePath = (OutputFileDirectory + String("WrapperErrors.txt")).CString();
        _ErrorFile = new ofstream(errorFilePath);
        PersistentAssert(!_ErrorFile->fail(), "Failed to open ErrorFile");
    } else {
        _InfoFile = new ::nullstream;
        _UnreportedFile = new ::nullstream;
        _ErrorFile = new ::nullstream;
    }
}
