/******************************************************************************
 * Header: SecureEngineCustomVM_FISH_EXTREME.h
 * Description:  Definitions for Private VM in SecureEngine
 *
 * Author/s: Oreans Technologies 
 * (c) 2014 Oreans Technologies
 *
 * --- File generated automatically from Oreans VM Generator (2/4/2014) ---
 ******************************************************************************/

// ***********************************************
// Definition of macros as function names
// ***********************************************

 #ifdef __cplusplus
  extern "C" {
 #endif

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000124_Start(void);

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000124_End(void);

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000125_Start(void);

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000125_End(void);

#ifdef __cplusplus
}
#endif


// ***********************************************
// x64 definition as function names
// ***********************************************

#if defined(PLATFORM_X64) && !defined(CV_X64_INSERT_VIA_INLINE)

#define VM_FISH_EXTREME_START CustomVM00000125_Start();
#define VM_FISH_EXTREME_END CustomVM00000125_End();

#define CV_CUSTOM_VMS_DEFINED

#endif 


// ***********************************************
// x32 definition as function names
// ***********************************************

#if defined(PLATFORM_X32) && !defined(CV_X32_INSERT_VIA_INLINE)

#define VM_FISH_EXTREME_START CustomVM00000124_Start();
#define VM_FISH_EXTREME_END CustomVM00000124_End();

#define CV_CUSTOM_VMS_DEFINED

#endif 


// ***********************************************
// x32/x64 definition as inline assembly
// ***********************************************

#ifndef CV_CUSTOM_VMS_DEFINED

#ifdef __BORLANDC__
  #include "SecureEngineCustomVM_BorlandC_inline_FISH_EXTREME.h"
#endif

#ifdef __GNUC__
  #include "SecureEngineCustomVM_GNU_inline_FISH_EXTREME.h"
#endif

#ifdef __ICL
  #include "SecureEngineCustomVM_ICL_inline_FISH_EXTREME.h"
#endif

#ifdef __LCC__
  #include "SecureEngineCustomVM_LCC_inline_FISH_EXTREME.h"
#endif

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
  #include "SecureEngineCustomVM_VC_inline_FISH_EXTREME.h"
#endif

#endif
