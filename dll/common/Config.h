/*
Config.h
Written by Matthew Fisher

Options that affect the compiler, debug output, DirectX debug modes, etc.
*/

//#define USE_EMPTY_CALLBACK
#define USE_D3DX

//
// VS 8+ includes some bounds checking even in release mode.  This include disable this and
// enables full optimization.
//
#ifndef _DEBUG
#define _SECURE_SCL 0
#endif

//
// Enables all standard C-runtime functions without warnings
//
#define _CRT_SECURE_NO_DEPRECATE

//
// commenting out the following lines will disable code involving the corresponding API.
// This can be useful if you lack some of the libraries involved with either API.
// Software rendering is always supported.
//
//#define USE_DIRECTX

//
// When defined, asserts are thrown on out-of-bounds vector access.
//
#ifdef _DEBUG
#define VECTOR_DEBUG
#endif

//
// Enables asserts in release mode
//
//#define ALWAYS_ASSERT

//
// Control over DirectX debug modes
//
#define D3D_DEBUG_INFO
//#define DEBUG_PS
//#define DEBUG_VS
//#define USE_REF

//
// Compression and PNG support.  PNG requires ZLIB
//
#define USE_ZLIB
#define USE_PNG

