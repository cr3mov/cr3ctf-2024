/******************************************************************************
 * Header: SecureEngineCustomVM_BorlandC_inline_FISH_EXTREME.h
 * Description: Borland C++ inline assembly macros definitions
 *
 * Author/s: Oreans Technologies 
 * (c) 2014 Oreans Technologies
 *
 * --- File generated automatically from Oreans VM Generator (2/4/2014) ---
 ******************************************************************************/

/***********************************************
 * Definition as inline assembly
 ***********************************************/

#ifdef PLATFORM_X32

#ifndef VM_FISH_EXTREME_START
#define VM_FISH_EXTREME_START                              __emit__ (0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x7C, 0x00, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20);
#endif

#ifndef VM_FISH_EXTREME_END
#define VM_FISH_EXTREME_END                                __emit__ (0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x0C, 0x02, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20);
#endif

#endif

#ifdef PLATFORM_X64

#ifndef VM_FISH_EXTREME_START
#define VM_FISH_EXTREME_START                              __emit__ (0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x7D, 0x00, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20);
#endif

#ifndef VM_FISH_EXTREME_END
#define VM_FISH_EXTREME_END                                __emit__ (0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x0D, 0x02, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20);
#endif

#endif

