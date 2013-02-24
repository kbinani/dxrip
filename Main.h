/*
Main.h
Written by Matthew Fisher

Main.h is included by all source files and includes every header file in the correct order.
*/
#pragma once

//
// Config.h includes a series of #defines used to control compiling options
//
#include "Config.h"

//
// Engine.h includes everything that rarely changes between applications, such as vector/Matrix4 libraries,
// OpenGL/DirectX graphics devices, software rasterizers, etc.
//
#include "Engine.h"

//
// MainControl.h includes everything that changes often between applications, such as what meshes to load,
// and also determines what is rendered each frame.
//
//#include "Controller.h"

//
// The App class glues the operating system, the graphics device, the application interface,
// and the window manager together.  It is essentially the first and the last code that is executed.
//
//#include "App.h"

#include "png.h"
#include "pngstruct.h"
namespace D3D9Base {
    #include <d3d9.h>
    #include <d3dx9.h>
}
using namespace D3D9Base;
#include "d3d9CallbackStructures.h"

extern "C" {
    #include "lua.h"
}
#include <stdint.h>
#include <fstream>
#include <map>
#include <vector>
