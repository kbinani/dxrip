/*
Main.cpp
Written by Matthew Fisher

Source file for precompiled header.
*/

#include "Main.h"

HANDLE globalDllModuleHandle = INVALID_HANDLE_VALUE;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    globalDllModuleHandle = hModule;
    return TRUE;
}
