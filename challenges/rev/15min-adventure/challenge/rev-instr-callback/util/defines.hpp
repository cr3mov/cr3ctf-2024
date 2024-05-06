#pragma once

/// \note es3n1n: Would require a bit of tweaks if we want to compile it with GCC (we don't)
#ifndef U_FORCEINL
    #define U_FORCEINL __forceinline
#endif

/// \note es3n1n: We might want to add xorstr here
#ifndef str_
    #define str_(s) (s)
#endif

/// \note es3n1n: A one byte key that we'll use for encryptions within the function communications
#define COMMUNICATION_ENC_KEY 0x66
#define COMMUNICATION_FLAG 0x49fa5962
