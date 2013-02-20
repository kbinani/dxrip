#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    extern "C" void *luaAlloc(void *ud, void *ptr, size_t osize, size_t nsize);

    class Context {
    public:
        ID3D9DeviceOverlay *screenOverlay;
        lua_State *state;

    public:
        static Context *Instance();

        ~Context();

    private:
        Context();
    };

} } }

#endif
