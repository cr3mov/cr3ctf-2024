#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <random>

#include <nand_nor.h>

#if defined(__clang__)
    #define NOOPT __attribute__((optnone))
#else
    #define NOOPT
#endif

namespace {
    using NumTy = std::int64_t;

    FORCEINLINE NumTy get_random_number() {
        std::random_device device;
        auto mt = std::mt19937_64(static_cast<uint64_t>(device()) << sizeof(uint32_t) * CHAR_BIT | device());

        using Limits = std::numeric_limits<NumTy>;
        std::uniform_int_distribution dist(Limits::min(), Limits::max());
        return dist(mt);
    }

    NOOPT NumTy OPTIMIZE_ME(const NumTy count) {
        NumTy result = 0;

        for (NumTy i = 0; i < count; ++i) {
            switch (i & 1) {
            case 0:
                result += i;
                break;
            case 1:
                result -= i;
                break;
            default:
                result *= 0;
                break;
            }
        }

        return result;
    }
} // namespace

int main() {
    std::cout << "Hallo, please wait while im looking for a seed." << std::endl;

    NumTy count = 0;
#ifndef _DEBUG
    do {
        count = get_random_number();

        /*
            In [6]: hex(uint64(0x6372337B5730777D) ^ uint64(0xde47307ebb83312a))
            Out[6]: '0xbd350305ecb34657'
         */
    } while (Xor(count, static_cast<NumTy>(0xde47307ebb83312a)) != 0xbd350305ecb34657);
#else
    //
#endif

    std::cout << "Seed has been found! Here is your flag: " << std::endl;

#ifndef _DEBUG
    volatile auto value = OPTIMIZE_ME(count);
#else
    volatile NumTy value = -3582923280968334336;
#endif
    std::array<NumTy, 2> values = {value, value};

    values[0] = static_cast<NumTy>(std::rotl(static_cast<std::uint64_t>(values[0]), static_cast<NumTy>(0x14)));
    values[0] ^= static_cast<NumTy>(0x9bfa8df4c0c069f2);
    values[0] >>= 0x0;
    values[0] ^= static_cast<NumTy>(0x8bb6fffffbffffff);

    values[1] = static_cast<NumTy>(std::rotl(static_cast<std::uint64_t>(values[1]), static_cast<NumTy>(0x6)));
    values[1] ^= static_cast<NumTy>(0x6e461b5bb97ccb93);
    values[1] >>= 0x0;
    values[1] ^= static_cast<NumTy>(0x82deffffffffffff);

    printf("%s\n", values.data());
    return 0;
}
