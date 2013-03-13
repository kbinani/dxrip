//
// Main.h
//
// Precompiled header for the D3D9 Wrapper
// Written by Matthew Fisher
//

#pragma once

#define INITGUID
#define NOMINMAX
#include "common/Config.h"

//
// Windows Header Files
//
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>
#include <crtdbg.h>

#include <fstream>

using namespace std;

namespace D3D9Base
{
#include <d3d9.h>
#include <dXErr.h>
#ifdef USE_D3DX
#include <d3dx9.h>
#endif
}

#include "Engine.h"

#include "d3d9CallbackStructures.h"
#include "Overlay.h"
#include "Globals.h"
#include "PointerSet.h"

namespace D3D9Wrapper
{
#include "d3d9Wrapper.h"
}

extern Globals g_Globals;