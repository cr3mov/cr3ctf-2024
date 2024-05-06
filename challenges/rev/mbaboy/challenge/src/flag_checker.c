#include "flag_checker.h"
#include <gb/drawing.h>
#include <gb/gb.h>

BANKREF_EXTERN(flag_checker)

/// \brief Check whether flag is valid or not
/// \note es3n1n: flag is `CR3{CU4K-2D7NY}`
uint8_t is_flag_valid(const char *flag, uint16_t length)
{
    int64_t x = *(int64_t *)(&flag[0]);
    int64_t y = *(int64_t *)(&flag[8]);
    int64_t len = (int64_t)length;

    volatile int64_t v3 = ~(~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)) & ~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)))) & ~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)))));
    volatile int64_t v7 = v3 & ~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)) & ~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)))) & ~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))) & ~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y))));
    volatile int64_t v4 = ~(~(~(~(~x & ~(~x & y)) & ~(y & ~(~x & y))) & ~(~(~(~x & ~(~x & y)) & ~(y & ~(~x & y))) & ~(x | ~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)))))) & ~(~(x | ~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)))) & ~(~(~(~x & ~(~x & y)) & ~(y & ~(~x & y))) & ~(x | ~(~(~y & y & ~(~y & y)) & ~(~y & ~(~y & y)))))));
    volatile int64_t v5 = ~v4 | ~(x | ~(~(v7 & ~(v7 & v3)) & ~(v3 & ~(v7 & v3))));
    volatile int64_t v8 = ~(~(v5 & ~(v5 & len)) & ~(len & ~(v5 & len)));

    int64_t expr = v8;                                                // (x ^ y) ^ len
    volatile int64_t expr2 = ~(~(v8 & ~(v8 & x)) & ~(x & ~(v8 & x))); // (expr ^ x)
    volatile int64_t expr3 = ~(~(v8 & ~(v8 & y)) & ~(y & ~(v8 & y))); // (expr ^ y)

    if (expr != 0x4b490c0d4c77607e)
        return 0;

    if (expr2 != 0x7d594e3744323d)
        return 1;

    if (expr3 != 0x4b3455437b335253)
        return 2;

    return 3;
}
