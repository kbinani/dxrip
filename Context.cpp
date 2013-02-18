#include "Main.h"
#include "Context.h"

namespace com { namespace github { namespace kbinani {

    Context::Context() {
        screenOverlay = NULL;
        countDrawIndexedPrimitive = 0;
        countDrawIndexedPrimitiveUP = 0;
        countDrawPrimitive = 0;
        countDrawPrimitiveUP = 0;
    }

    Context &Context::Instance() {
        static Context context;
        return context;
    }

    void Context::ReportDrawCallbackStatistics() {
        screenOverlay->WriteLine(
            String("DrawIndexedPrimitive=") + String(countDrawIndexedPrimitive) + String("; ") +
            String("DrawIndexedPrimitiveUP=") + String(countDrawIndexedPrimitiveUP) + String("; ") +
            String("DrawPrimitive=") + String(countDrawPrimitive) + String("; ") +
            String("DrawPrimitiveUP=") + String(countDrawPrimitiveUP), RGBColor(34, 177, 76), 0);
    }

} } }
