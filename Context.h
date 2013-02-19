#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class Context {
    public:
        ID3D9DeviceOverlay *screenOverlay;

    public:
        static Context *Instance();

    private:
        Context();
    };

} } }

#endif
