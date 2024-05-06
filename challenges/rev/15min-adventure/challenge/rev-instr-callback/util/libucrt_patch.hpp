#pragma once
#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY
#include <Windows.h>

namespace libcrt {
    void argc_callback();
}

extern "C" {
    int __argc = 0; // The number of arguments in __argv or __wargv
    char** __argv = nullptr; // The arguments as narrow strings
    wchar_t** __wargv = nullptr; // The arguments as wide strings
    char* _pgmptr = nullptr; // The name of the program as a narrow string
    wchar_t* _wpgmptr = nullptr; // The name of the program as a wide string
    char* _acmdln = nullptr; // The raw command line as a narrow string
    wchar_t* _wcmdln = nullptr; // The raw command line as a wide string

    bool __cdecl __acrt_initialize_command_line() {
        _acmdln = GetCommandLineA();
        _wcmdln = GetCommandLineW();
        return 1;
    }

    bool __cdecl __acrt_uninitialize_command_line(bool const /* terminating */) {
        return true;
    }

    void libucrt_argc_callback(void) {
        libcrt::argc_callback();
    }
}
