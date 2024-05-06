#include <cstdint>
#include <cstdio>
#include <iostream>

#define CT_RANDOM                    \
    std::integral_constant<uint32_t, \
                           uint32_t(__TIMESTAMP__[0]) + uint32_t(__TIMESTAMP__[1]) + uint32_t(__TIMESTAMP__[2]) + uint32_t(__TIMESTAMP__[3]) + 2>::value

#if defined(__linux__) || defined(__APPLE__)
    #define GCC_ASM
    #define DECOMP_BREAK_DISABLE_GHIDRA_RAW \
        asm("movq $-1, %r10");              \
        asm("leaq (%r10), %r10");           \
        asm("movq (%r10), %r11");

    #define DECOMP_BREAK_DISABLE_IDA_RAW asm("enter $0xFFFF, $0xFF");
#else
    #define DECOMP_BREAK_DISABLE_GHIDRA_RAW \
        __asm { mov r10, -1 }                \
        __asm {                             \
            lea r10, qword ptr[r10]         \
        }                                   \
        __asm { mov r11, qword ptr [r10] }

    #define DECOMP_BREAK_DISABLE_IDA_RAW __asm { enter 0xFFFF, 0xFF }
#endif

#define DECOMP_BREAK_DISABLE_ALL_RAW \
    DECOMP_BREAK_DISABLE_GHIDRA_RAW; \
    DECOMP_BREAK_DISABLE_IDA_RAW;

#define DECOMP_BREAK_DISABLE_GHIDRA     \
    if (num == CT_RANDOM) {             \
        DECOMP_BREAK_DISABLE_GHIDRA_RAW \
    }
#define DECOMP_BREAK_DISABLE_IDA     \
    if (num == CT_RANDOM) {          \
        DECOMP_BREAK_DISABLE_IDA_RAW \
    }

#define DECOMP_BREAK_ALL         \
    DECOMP_BREAK_DISABLE_GHIDRA; \
    DECOMP_BREAK_DISABLE_IDA;

#if defined(__clang__)
    #define NOOPT __attribute__((optnone))
#else
    #define NOOPT
#endif

#define FORCEINL __attribute__((always_inline))

namespace {
    template <uint32_t key_minus_one>
    FORCEINL uint32_t decrypt_var(uint32_t var) {
        uint32_t cpy = var;
        uint32_t key = key_minus_one;
#if defined(GCC_ASM)
        asm("stc");
        asm("adc %1, %0" : "+r"(cpy) : "r"(key));
#else
        __asm { stc }
        __asm {
            mov r12, key
        }
        __asm { adc cpy, r12 }
#endif
        return (uint32_t)cpy;
    }
} // namespace

int main() {
    volatile uint32_t num = 1;
    DECOMP_BREAK_ALL;

    printf("flag: ");

    char flag[65] = {0};
    std::scanf("%64s", flag);

    volatile std::int64_t valid = 0;

    DECOMP_BREAK_ALL;
    if (((flag[0] - decrypt_var<0x1c>(-0x17)) ^ 0) != decrypt_var<0x13>(0x49)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[1] - decrypt_var<0x6>(0x3)) ^ flag[0]) != decrypt_var<0x6>(0x4)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[2] - decrypt_var<0x17>(0x1)) ^ flag[1]) != decrypt_var<0x18>(0x4f)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[3] - decrypt_var<0x19>(-0xd)) ^ flag[2]) != decrypt_var<0x11>(0x4b)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[4] - decrypt_var<0xa>(0x1)) ^ flag[3]) != decrypt_var<0x1a>(0x8)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[5] - decrypt_var<0x1c>(-0xd)) ^ flag[4]) != decrypt_var<0x1c>(0x27)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[6] - decrypt_var<0xd>(0x10)) ^ flag[5]) != decrypt_var<0xb>(0x54)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[7] - decrypt_var<0xf>(-0xa)) ^ flag[6]) != decrypt_var<0x1a>(-0x1b)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[8] - decrypt_var<0xe>(-0x6)) ^ flag[7]) != decrypt_var<0xb>(0x16)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[9] - decrypt_var<0x9>(-0x5)) ^ flag[8]) != decrypt_var<0x8>(0x27)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[10] - decrypt_var<0xc>(0xf)) ^ flag[9]) != decrypt_var<0x9>(0x18)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[11] - decrypt_var<0x1b>(-0x16)) ^ flag[10]) != decrypt_var<0x8>(0x34)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[12] - decrypt_var<0x14>(-0xd)) ^ flag[11]) != decrypt_var<0xb>(0x6c)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[13] - decrypt_var<0xd>(0x9)) ^ flag[12]) != decrypt_var<0x7>(0x60)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[14] - decrypt_var<0x6>(-0x2)) ^ flag[13]) != decrypt_var<0x14>(0x19)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[15] - decrypt_var<0xf>(-0x6)) ^ flag[14]) != decrypt_var<0x9>(0x5b)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[16] - decrypt_var<0x6>(0x12)) ^ flag[15]) != decrypt_var<0xe>(0x4f)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[17] - decrypt_var<0x8>(0xe)) ^ flag[16]) != decrypt_var<0x13>(0x6b)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[18] - decrypt_var<0x1b>(-0xa)) ^ flag[17]) != decrypt_var<0xa>(0x72)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[19] - decrypt_var<0xa>(0x7)) ^ flag[18]) != decrypt_var<0x1e>(0x4c)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[20] - decrypt_var<0x19>(-0x9)) ^ flag[19]) != decrypt_var<0x14>(0x1d)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[21] - decrypt_var<0x7>(0xd)) ^ flag[20]) != decrypt_var<0x8>(0x63)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[22] - decrypt_var<0x1e>(-0x15)) ^ flag[21]) != decrypt_var<0x1d>(0x55)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[23] - decrypt_var<0x7>(0xf)) ^ flag[22]) != decrypt_var<0x5>(0x4a)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[24] - decrypt_var<0x19>(0x4)) ^ flag[23]) != decrypt_var<0xb>(0x5b)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[25] - decrypt_var<0x1c>(-0x3)) ^ flag[24]) != decrypt_var<0x7>(0x23)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[26] - decrypt_var<0x6>(0x14)) ^ flag[25]) != decrypt_var<0x1e>(0x18)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[27] - decrypt_var<0x12>(-0xb)) ^ flag[26]) != decrypt_var<0x18>(0x1a)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[28] - decrypt_var<0x8>(0xe)) ^ flag[27]) != decrypt_var<0x5>(0x67)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[29] - decrypt_var<0x13>(0x1)) ^ flag[28]) != decrypt_var<0x16>(0x53)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[30] - decrypt_var<0xb>(0x7)) ^ flag[29]) != decrypt_var<0xa>(0x18)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[31] - decrypt_var<0x15>(-0x8)) ^ flag[30]) != decrypt_var<0x17>(-0x18)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[32] - decrypt_var<0x10>(-0x6)) ^ flag[31]) != decrypt_var<0x10>(-0xa)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[33] - decrypt_var<0xb>(0x12)) ^ flag[32]) != decrypt_var<0x19>(0x36)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[34] - decrypt_var<0xd>(0x9)) ^ flag[33]) != decrypt_var<0x19>(-0x8)) {
        valid -= 1;
    } else {
        valid += 1;
    }
    DECOMP_BREAK_ALL;
    if (((flag[35] - decrypt_var<0x7>(0x2)) ^ flag[34]) != decrypt_var<0x9>(0x11)) {
        valid -= 1;
    } else {
        valid += 1;
    }

    const char* message = valid == decrypt_var<0x1d>(0x6) ? "that's right!\n" : "nope\n";
    printf(message);

    return 0;
}
