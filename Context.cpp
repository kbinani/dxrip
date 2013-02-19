#include "Main.h"
#include "Context.h"

namespace com { namespace github { namespace kbinani {

    Context::Context() {
        screenOverlay = NULL;
    }

    Context *Context::Instance() {
        static Context context;
        return &context;
    }

} } }
