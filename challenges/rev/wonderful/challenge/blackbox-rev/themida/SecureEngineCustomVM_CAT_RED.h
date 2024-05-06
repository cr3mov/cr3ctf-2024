/******************************************************************************
 * Header: SecureEngineCustomVM_CAT_RED.h
 * Description:  Definitions for Private VM in SecureEngine
 *
 * Author/s: Oreans Technologies 
 * (c) 2016 Oreans Technologies
 *
 * --- File generated automatically from Oreans VM Generator (8/1/2016) ---
 ******************************************************************************/

// ***********************************************
// Definition of macros as function names
// ***********************************************

 #ifdef __cplusplus
  extern "C" {
 #endif

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000154_Start(void);

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000154_End(void);

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000155_Start(void);

DLL_IMPORT void STDCALL_CONVENTION CustomVM00000155_End(void);

#ifdef __cplusplus
}
#endif


// ***********************************************
// x64 definition as function names
// ***********************************************

#if defined(PLATFORM_X64) && !defined(CV_X64_INSERT_VIA_INLINE)

#define VM_CAT_RED_START CustomVM00000155_Start();
#define VM_CAT_RED_END CustomVM00000155_End();

#define CV_CUSTOM_VMS_DEFINED

#endif 


// ***********************************************
// x32 definition as function names
// ***********************************************

#if defined(PLATFORM_X32) && !defined(CV_X32_INSERT_VIA_INLINE)

#define VM_CAT_RED_START CustomVM00000154_Start();
#define VM_CAT_RED_END CustomVM00000154_End();

#define CV_CUSTOM_VMS_DEFINED

#endif 


// ***********************************************
// x32/x64 definition as inline assembly
// ***********************************************

#ifndef CV_CUSTOM_VMS_DEFINED

#ifdef __BORLANDC__
  #include "SecureEngineCustomVM_BorlandC_inline_CAT_RED.h"
#endif

#ifdef __GNUC__
  #include "SecureEngineCustomVM_GNU_inline_CAT_RED.h"
#endif

#ifdef __ICL
  #include "SecureEngineCustomVM_ICL_inline_CAT_RED.h"
#endif

#ifdef __LCC__
  #include "SecureEngineCustomVM_LCC_inline_CAT_RED.h"
#endif

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
  #include "SecureEngineCustomVM_VC_inline_CAT_RED.h"
#endif

#endif
