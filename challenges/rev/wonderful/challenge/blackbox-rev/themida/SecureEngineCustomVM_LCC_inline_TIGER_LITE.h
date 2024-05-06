/******************************************************************************
 * Header: SecureEngineCustomVM_LCC_inline_TIGER_LITE.h
 * Description: LCC inline assembly macros definitions
 *
 * Author/s: Oreans Technologies 
 * (c) 2019 Oreans Technologies
 *
 * --- File generated automatically from Oreans VM Generator (29/10/2019) ---
 ******************************************************************************/

/***********************************************
 * Definition as inline assembly
 ***********************************************/

#ifdef PLATFORM_X32

#ifndef VM_TIGER_LITE_START
#define VM_TIGER_LITE_START                                __asm__ (" .byte\t0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x9E, 0x00, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20");
#endif

#ifndef VM_TIGER_LITE_END
#define VM_TIGER_LITE_END                                  __asm__ (" .byte\t0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x2E, 0x02, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20");
#endif

#endif

#ifdef PLATFORM_X64

#ifndef VM_TIGER_LITE_START
#define VM_TIGER_LITE_START                                __asm__ (" .byte\t0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x9F, 0x00, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20");
#endif

#ifndef VM_TIGER_LITE_END
#define VM_TIGER_LITE_END                                  __asm__ (" .byte\t0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x2F, 0x02, 0x00, 0x00, \
                                                                     0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20");
#endif

#endif

