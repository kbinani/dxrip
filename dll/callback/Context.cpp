#include "Main.h"
#include "Context.h"

namespace com { namespace github { namespace kbinani {

    Context::Context() {
        screenOverlay = NULL;
        device = NULL;
        sceneCount = 0;
    }

    Context *Context::Instance() {
        static Context context;
        return &context;
    }

    Context::~Context() {
        screenOverlay = NULL;
    }

} } }
