#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <array>

enum Rotation
{
    U = 2, Up = -2, U2 = 4,
    D = 3, Dp = -3, L2 = 9,
    F = 5, Fp = -5, F2 = 25,
    B = 7, Bp = -7, R2 = 49,
    L = 11, Lp = -11, B2 = 121,
    R = 13, Rp = -13, D2 = 169
};

class Cube
{
public:
    Cube();
    Cube(const Cube&);
    Cube(uint32_t sides[6]);

    Cube& operator=(const Cube&);

    friend std::ostream& operator<<(std::ostream&, const Cube&);

    void up(bool reverse = false);
    void down(bool reverse = false);
    void left(bool reverse = false);
    void right(bool reverse = false);
    void front(bool reverse = false);
    void back(bool reverse = false);

    void combo_move(std::list<Rotation>&);
    void scramble();
    uint32_t get_val(size_t, size_t);
    uint32_t get_side(size_t);
    size_t get_corner_index(size_t);

    bool on_group(size_t group);
    bool on_group1();
    bool on_group2();
    bool on_group3();
    bool solved();
    bool check_parity();

    uint32_t sides[6] = { 0, 0, 0, 0, 0, 0 };
private:
    // Corner orientations and colors:
    // 0   1   2   3   4   5   6   7
    // UBL UBR UFR UFL F2L B2L B2R F2R
    uint32_t corners[8][3] = {
        {0, 1, 4}, {0, 3, 4}, {0, 2, 3}, {0, 1, 2}, {1, 2, 5}, {1, 4, 5}, {3, 4, 5}, {2, 3, 5}
    };

    void rotate(size_t);
    void set_val(size_t, size_t, uint32_t);

    uint32_t get_val_by_ij(size_t, size_t, size_t) const;
};
