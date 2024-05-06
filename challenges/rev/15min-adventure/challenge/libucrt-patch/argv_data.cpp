#undef __argc
#undef __argv
#undef __wargv
#undef _pgmptr
#undef _wpgmptr
#undef _acmdln
#undef _wcmdln

#define _LCRT_DEFINE_IAT_SYMBOL(f)

char* __stdcall GetCommandLineA(void);
wchar_t* __stdcall GetCommandLineW(void);

extern "C" {
    extern int       __argc;       // The number of arguments in __argv or __wargv
    extern char** __argv; // The arguments as narrow strings
    extern wchar_t** __wargv; // The arguments as wide strings
    extern char* _pgmptr; // The name of the program as a narrow string
    extern wchar_t* _wpgmptr; // The name of the program as a wide string
    extern char* _acmdln; // The raw command line as a narrow string
    extern wchar_t* _wcmdln; // The raw command line as a wide string

    extern void libucrt_argc_callback(void);

    int* __cdecl __p___argc() { libucrt_argc_callback();  return &__argc; }
    _LCRT_DEFINE_IAT_SYMBOL(__p___argc);

    char*** __cdecl __p___argv() { return &__argv; }
    _LCRT_DEFINE_IAT_SYMBOL(__p___argv);

    wchar_t*** __cdecl __p___wargv() { return &__wargv; }
    _LCRT_DEFINE_IAT_SYMBOL(__p___wargv);

    char** __cdecl __p__pgmptr() { return &_pgmptr; }
    _LCRT_DEFINE_IAT_SYMBOL(__p__pgmptr);

    wchar_t** __cdecl __p__wpgmptr() { return &_wpgmptr; }
    _LCRT_DEFINE_IAT_SYMBOL(__p__wpgmptr);

    char** __cdecl __p__acmdln() { return &_acmdln; }
    _LCRT_DEFINE_IAT_SYMBOL(__p__acmdln);

    wchar_t** __cdecl __p__wcmdln() { return &_wcmdln; }
    _LCRT_DEFINE_IAT_SYMBOL(__p__wcmdln);
} // extern "C"