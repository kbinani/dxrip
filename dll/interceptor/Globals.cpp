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
		std::ostringstream stream;
		stream << OutputFileDirectory.CString() << "WrapperInfo.txt";
		std::ostringstream ss;
		ss << "Failed to open InfoFile; OutputFileDirectory=" << OutputFileDirectory.CString() << "; String(\"WrapperInfo.txt\")=" << (String("WrapperInfo.txt").CString()) << "; path=" << stream.str();
        _InfoFile = new ofstream(stream.str().c_str());

		std::string unreportedFilePath = std::string(OutputFileDirectory.CString()) + std::string("WrapperUnreportedEvents.txt");
		_UnreportedFile = new ofstream(unreportedFilePath.c_str());
        PersistentAssert(!_UnreportedFile->fail(), "Failed to open UnreportedFile");

		std::string errorFilePath = std::string(OutputFileDirectory.CString()) + std::string("WrapperErrors.txt");
        _ErrorFile = new ofstream(errorFilePath.c_str());
        PersistentAssert(!_ErrorFile->fail(), "Failed to open ErrorFile");
    } else {
        _InfoFile = new ::nullstream;
        _UnreportedFile = new ::nullstream;
        _ErrorFile = new ::nullstream;
    }
}
