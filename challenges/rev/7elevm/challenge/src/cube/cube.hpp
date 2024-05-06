#pragma once
#include <array>
#include <iostream>
#include <list>
#include <vector>

#include "util/platform.hpp"

class Cube {
public:
    constexpr Cube() = default;
    ~Cube() = default;

    FORCEINL void up(bool reverse) {
        for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
            rotate(0);
            uint32_t mask = 0x00000FFF;
            uint32_t temp = sides[1] & mask; // LEFT side
            for (size_t j = 1; j < 4; ++j) {
                sides[j] &= ~mask;
                sides[j] |= sides[j + 1] & mask;
            }
            sides[4] &= ~mask;
            sides[4] |= temp;
        }
    }

    FORCEINL void down(bool reverse) {
        for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
            rotate(5);
            uint32_t mask = 0x0FFF0000;
            uint32_t temp = sides[4] & mask; // BACK side
            for (size_t j = 4; j > 1; --j) {
                sides[j] &= ~mask;
                sides[j] |= sides[j - 1] & mask;
            }
            sides[1] &= ~mask;
            sides[1] |= temp;
        }
    }

    FORCEINL void left(bool reverse) {
        for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
            rotate(1);
            uint32_t t[3];
            for (size_t i = 0; i < 3; ++i)
                t[i] = get_val(4, i + 2);
            for (size_t i = 6; i <= 8; ++i) {
                set_val(4, i - 4, get_val(5, i % 8));
                set_val(5, i % 8, get_val(2, i % 8));
                set_val(2, i % 8, get_val(0, i % 8));
                set_val(0, i % 8, t[i - 6]);
            }
        }
    }

    FORCEINL void right(bool reverse) {
        for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
            rotate(3);
            uint32_t t[3];
            for (size_t i = 0; i < 3; ++i)
                t[i] = get_val(4, (i + 6) % 8);
            for (size_t i = 2; i <= 4; ++i) {
                set_val(4, (i + 4) % 8, get_val(0, i));
                set_val(0, i, get_val(2, i));
                set_val(2, i, get_val(5, i));
                set_val(5, i, t[i - 2]);
            }
        }
    }

    FORCEINL void front(bool reverse) {
        for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
            rotate(2);
            uint32_t t[3];
            for (size_t i = 0; i < 3; ++i)
                t[i] = get_val(0, i + 4);
            for (size_t i = 0; i < 3; ++i) {
                set_val(0, i + 4, get_val(1, i + 2));
                set_val(1, i + 2, get_val(5, i));
                set_val(5, i, get_val(3, (i + 6) % 8));
                set_val(3, (i + 6) % 8, t[i]);
            }
        }
    }

    FORCEINL void back(bool reverse) {
        for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
            rotate(4);
            uint32_t t[3];
            for (size_t i = 0; i < 3; ++i)
                t[i] = get_val(0, i);
            for (size_t i = 0; i < 3; ++i) {
                set_val(0, i, get_val(3, i + 2));
                set_val(3, i + 2, get_val(5, i + 4));
                set_val(5, i + 4, get_val(1, (i + 6) % 8));
                set_val(1, (i + 6) % 8, t[i]);
            }
        }
    }

    [[nodiscard]] NOINL uint32_t get_val(size_t side, size_t index) {
        return (sides[side] & (0x0000000F << (index * 4))) >> (index * 4);
    }

    uint32_t sides[6] = {0, 0, 0, 0, 0, 0};

private:
    // clockwise rotation
    NOINL void rotate(size_t side) {
        uint32_t temp = (sides[side] & 0xFF000000) >> 24;
        sides[side] <<= 8;
        sides[side] |= temp;
    }

    NOINL void set_val(size_t side, size_t index, uint32_t new_val) {
        sides[side] &= ~(0x0000000F << (index * 4));
        sides[side] |= new_val << (index * 4);
    }
};
