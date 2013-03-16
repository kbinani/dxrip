#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Main.h"
#include "Config.h"

namespace dxrip { namespace callback {

    class Context {
    public:
        ID3D9DeviceOverlay *screenOverlay;
        LPDIRECT3DDEVICE9 device;
        int sceneCount;

    private:
        Config *config;

    public:
        static Context *Instance();

        ~Context();

        const Config &Config() const;

    private:
        Context();

        const std::string GetModuleFileDirectory(HANDLE dllHandle) const;
    };

} }

#endif
