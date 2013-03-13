#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class Context {
    public:
        ID3D9DeviceOverlay *screenOverlay;
        LPDIRECT3DDEVICE9 device;
        int sceneCount;

    public:
        static Context *Instance();

        ~Context();

    private:
        Context();
    };

} } }

#endif
