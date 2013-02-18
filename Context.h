#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class Context {
    public:
        ID3D9DeviceOverlay *screenOverlay;
        int countDrawPrimitive;
        int countDrawIndexedPrimitive;
        int countDrawPrimitiveUP;
        int countDrawIndexedPrimitiveUP;

    public:
        static Context &Instance();

        void ReportDrawCallbackStatistics();

    private:
        Context();
    };

} } }

#endif
