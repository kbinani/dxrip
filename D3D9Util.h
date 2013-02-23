#pragma once

#include "Main.h"

namespace com { namespace github { namespace kbinani {

    class D3D9Util {
    public:
        static std::string GetFVFString(DWORD fvf, std::string separator = ",") {
            std::string result = "";
            if ((fvf & D3DFVF_RESERVED0) == D3DFVF_RESERVED0) result += separator + "D3DFVF_RESERVED0";
            if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_POSITION_MASK) result += separator + "D3DFVF_POSITION_MASK";
            if ((fvf & D3DFVF_XYZ) == D3DFVF_XYZ) result += separator + "D3DFVF_XYZ";
            if ((fvf & D3DFVF_XYZRHW) == D3DFVF_XYZRHW) result += separator + "D3DFVF_XYZRHW";
            if ((fvf & D3DFVF_XYZB1) == D3DFVF_XYZB1) result += separator + "D3DFVF_XYZB1";
            if ((fvf & D3DFVF_XYZB2) == D3DFVF_XYZB2) result += separator + "D3DFVF_XYZB2";
            if ((fvf & D3DFVF_XYZB3) == D3DFVF_XYZB3) result += separator + "D3DFVF_XYZB3";
            if ((fvf & D3DFVF_XYZB4) == D3DFVF_XYZB4) result += separator + "D3DFVF_XYZB4";
            if ((fvf & D3DFVF_XYZB5) == D3DFVF_XYZB5) result += separator + "D3DFVF_XYZB5";
            if ((fvf & D3DFVF_XYZW) == D3DFVF_XYZW) result += separator + "D3DFVF_XYZW";

            if ((fvf & D3DFVF_NORMAL) == D3DFVF_NORMAL) result += separator + "D3DFVF_NORMAL";
            if ((fvf & D3DFVF_PSIZE) == D3DFVF_PSIZE) result += separator + "D3DFVF_PSIZE";
            if ((fvf & D3DFVF_DIFFUSE) == D3DFVF_DIFFUSE) result += separator + "D3DFVF_DIFFUSE";
            if ((fvf & D3DFVF_SPECULAR) == D3DFVF_SPECULAR) result += separator + "D3DFVF_SPECULAR";

            if ((fvf & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEXCOUNT_MASK) result += separator + "D3DFVF_TEXCOUNT_MASK";
            if ((fvf & D3DFVF_TEXCOUNT_SHIFT) == D3DFVF_TEXCOUNT_SHIFT) result += separator + "D3DFVF_TEXCOUNT_SHIFT";
            if ((fvf & D3DFVF_TEX0) == D3DFVF_TEX0) result += separator + "D3DFVF_TEX0";
            if ((fvf & D3DFVF_TEX1) == D3DFVF_TEX1) result += separator + "D3DFVF_TEX1";
            if ((fvf & D3DFVF_TEX2) == D3DFVF_TEX2) result += separator + "D3DFVF_TEX2";
            if ((fvf & D3DFVF_TEX3) == D3DFVF_TEX3) result += separator + "D3DFVF_TEX3";
            if ((fvf & D3DFVF_TEX4) == D3DFVF_TEX4) result += separator + "D3DFVF_TEX4";
            if ((fvf & D3DFVF_TEX5) == D3DFVF_TEX5) result += separator + "D3DFVF_TEX5";
            if ((fvf & D3DFVF_TEX6) == D3DFVF_TEX6) result += separator + "D3DFVF_TEX6";
            if ((fvf & D3DFVF_TEX7) == D3DFVF_TEX7) result += separator + "D3DFVF_TEX7";
            if ((fvf & D3DFVF_TEX8) == D3DFVF_TEX8) result += separator + "D3DFVF_TEX8";

            if ((fvf & D3DFVF_LASTBETA_UBYTE4) == D3DFVF_LASTBETA_UBYTE4) result += separator + "D3DFVF_LASTBETA_UBYTE4";
            if ((fvf & D3DFVF_LASTBETA_D3DCOLOR) == D3DFVF_LASTBETA_D3DCOLOR) result += separator + "D3DFVF_LASTBETA_D3DCOLOR";

            if ((fvf & D3DFVF_RESERVED2) == D3DFVF_RESERVED2) result += separator + "D3DFVF_RESERVED2";

            string::size_type pos = result.find_first_of(separator);
            if (pos != string::npos) {
                result = result.substr(pos + separator.size());
            }

            return result;
        }
    };

} } }
