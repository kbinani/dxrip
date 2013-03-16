#include "Main.h"
#include "Context.h"
#include "Config.h"

namespace dxrip { namespace callback {

    Context::Context() {
        screenOverlay = NULL;
        device = NULL;
        sceneCount = 0;

        std::string directory = GetModuleFileDirectory(globalDllModuleHandle);
        config = new dxrip::callback::Config(directory);
    }

    Context *Context::Instance() {
        static Context context;
        return &context;
    }

    const Config &Context::Config() const {
        return *config;
    }

    Context::~Context() {
        screenOverlay = NULL;
        if (config) {
            delete config;
            config = NULL;
        }
    }

    const std::string Context::GetModuleFileDirectory(HANDLE dllHandle) const {
        char fileName[_MAX_PATH + 1] = "";
        GetModuleFileNameA((HMODULE)dllHandle, fileName, _MAX_PATH);
        char driveName[_MAX_DRIVE + 1] = "";
        char directoryName[_MAX_DIR + 1] = "";
        ::_splitpath_s(fileName,
                       driveName, _MAX_DRIVE,
                       directoryName, _MAX_DIR,
                       NULL, 0,
                       NULL, 0);
        return std::string(driveName) + std::string("") + std::string(directoryName);
    }

} }
