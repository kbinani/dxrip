#include "Main.h"

void Globals::Init()
{
	Initialized = true;
	
    UsingNullPixelShader = false;
	ParameterFile Parameters(String("WrapperParameters.txt"));
	BaseDirectory = Parameters.GetRequiredString("BaseDirectory");
	OutputFileDirectory = BaseDirectory + String("\\") + Parameters.GetRequiredString("OutputFileDirectory") + String("\\");
	RealD3D9DLL = Parameters.GetRequiredString("RealD3D9DLL");
	
	UsingOverlay = Parameters.GetBoolean("UsingOverlay");
	ConsoleLineCount = Parameters.GetUnsignedInteger("ConsoleLineCount");

    ForceSoftwareVertexProcessing = Parameters.GetBoolean("ForceSoftwareVertexProcessing");
    IgnoreTextureLocks = Parameters.GetBoolean("IgnoreTextureLocks");

    VideoCaptureMode = Parameters.GetBoolean("VideoCaptureMode");
    if(VideoCaptureMode)
    {
        IgnoreTextureLocks = true;
    }

#ifdef WRITING_TO_FILES
    _InfoFile = new ofstream;
    _InfoFile->open((OutputFileDirectory + String("WrapperInfo.txt")).CString());
    PersistentAssert(!_InfoFile->fail(), "Failed to open InfoFile");

    _UnreportedFile = new ofstream;
    _UnreportedFile->open((OutputFileDirectory + String("WrapperUnreportedEvents.txt")).CString());
    PersistentAssert(!_UnreportedFile->fail(), "Failed to open UnreportedFile");

    _ErrorFile = new ofstream;
    _ErrorFile->open((OutputFileDirectory + String("WrapperErrors.txt")).CString());
    PersistentAssert(!_ErrorFile->fail(), "Failed to open ErrorFile");
#endif
}
