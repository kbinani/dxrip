#pragma once

#include "Main.h"

namespace dxrip { namespace callback {

    class D3D9Util {
    public:
        static std::string GetFVFString(DWORD fvf, std::string separator = ",") {
            std::string result = "";
            if ((fvf & D3DFVF_RESERVED0) == D3DFVF_RESERVED0) result += separator + "RESERVED0";
            if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_POSITION_MASK) result += separator + "POSITION_MASK";
            if ((fvf & D3DFVF_XYZ) == D3DFVF_XYZ) result += separator + "XYZ";
            if ((fvf & D3DFVF_XYZRHW) == D3DFVF_XYZRHW) result += separator + "XYZRHW";
            if ((fvf & D3DFVF_XYZB1) == D3DFVF_XYZB1) result += separator + "XYZB1";
            if ((fvf & D3DFVF_XYZB2) == D3DFVF_XYZB2) result += separator + "XYZB2";
            if ((fvf & D3DFVF_XYZB3) == D3DFVF_XYZB3) result += separator + "XYZB3";
            if ((fvf & D3DFVF_XYZB4) == D3DFVF_XYZB4) result += separator + "XYZB4";
            if ((fvf & D3DFVF_XYZB5) == D3DFVF_XYZB5) result += separator + "XYZB5";
            if ((fvf & D3DFVF_XYZW) == D3DFVF_XYZW) result += separator + "XYZW";

            if ((fvf & D3DFVF_NORMAL) == D3DFVF_NORMAL) result += separator + "NORMAL";
            if ((fvf & D3DFVF_PSIZE) == D3DFVF_PSIZE) result += separator + "PSIZE";
            if ((fvf & D3DFVF_DIFFUSE) == D3DFVF_DIFFUSE) result += separator + "DIFFUSE";
            if ((fvf & D3DFVF_SPECULAR) == D3DFVF_SPECULAR) result += separator + "SPECULAR";

            if ((fvf & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEXCOUNT_MASK) result += separator + "TEXCOUNT_MASK";
            if ((fvf & D3DFVF_TEXCOUNT_SHIFT) == D3DFVF_TEXCOUNT_SHIFT) result += separator + "TEXCOUNT_SHIFT";
            if ((fvf & D3DFVF_TEX0) == D3DFVF_TEX0) result += separator + "TEX0";
            if ((fvf & D3DFVF_TEX1) == D3DFVF_TEX1) result += separator + "TEX1";
            if ((fvf & D3DFVF_TEX2) == D3DFVF_TEX2) result += separator + "TEX2";
            if ((fvf & D3DFVF_TEX3) == D3DFVF_TEX3) result += separator + "TEX3";
            if ((fvf & D3DFVF_TEX4) == D3DFVF_TEX4) result += separator + "TEX4";
            if ((fvf & D3DFVF_TEX5) == D3DFVF_TEX5) result += separator + "TEX5";
            if ((fvf & D3DFVF_TEX6) == D3DFVF_TEX6) result += separator + "TEX6";
            if ((fvf & D3DFVF_TEX7) == D3DFVF_TEX7) result += separator + "TEX7";
            if ((fvf & D3DFVF_TEX8) == D3DFVF_TEX8) result += separator + "TEX8";

            if ((fvf & D3DFVF_LASTBETA_UBYTE4) == D3DFVF_LASTBETA_UBYTE4) result += separator + "LASTBETA_UBYTE4";
            if ((fvf & D3DFVF_LASTBETA_D3DCOLOR) == D3DFVF_LASTBETA_D3DCOLOR) result += separator + "LASTBETA_D3DCOLOR";

            if ((fvf & D3DFVF_RESERVED2) == D3DFVF_RESERVED2) result += separator + "RESERVED2";

            string::size_type pos = result.find_first_of(separator);
            if (pos != string::npos) {
                result = result.substr(pos + separator.size());
            }

            return result;
        }
    };

} }