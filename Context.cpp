#include "Main.h"
#include "Context.h"

namespace com { namespace github { namespace kbinani {

    Context::Context() {
        screenOverlay = NULL;
        device = NULL;
        state = lua_newstate(&luaAlloc, NULL);
        sceneCount = 0;
    }

    Context *Context::Instance() {
        static Context context;
        return &context;
    }

    Context::~Context() {
        screenOverlay = NULL;
        if (state) lua_close(state);
    }

    extern "C" void *luaAlloc(void *ud, void *ptr, size_t osize, size_t nsize) {
        if (nsize == 0) {
            free(ptr);
            return NULL;
        } else {
            return realloc(ptr, nsize);
        }
    }

} } }
