#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <string>
#include <array>
#include <random>

#include <cassert>

#include "cube.hpp"


enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRONT,
	BACK
};

struct data_t {
	int type;
	bool reverse;
};

data_t rotations[] = {
data_t(BACK, true),
data_t(RIGHT, true),
data_t(UP, true),
data_t(RIGHT, false),
data_t(UP, false),
data_t(LEFT, false),
data_t(RIGHT, false),
data_t(FRONT, false),
data_t(DOWN, false),
data_t(RIGHT, false),
data_t(DOWN, true),
data_t(LEFT, false),
data_t(FRONT, true),
data_t(LEFT, true),
data_t(DOWN, true),
data_t(RIGHT, false),
data_t(LEFT, false),
data_t(LEFT, false),
data_t(UP, true),
data_t(UP, true),
data_t(DOWN, false),
data_t(DOWN, true),
data_t(FRONT, true),
data_t(RIGHT, true),
data_t(UP, true),
data_t(RIGHT, true),
data_t(DOWN, true),
data_t(BACK, true),
data_t(LEFT, false),
data_t(LEFT, false),
data_t(UP, true),
data_t(FRONT, true),
data_t(DOWN, true),
data_t(RIGHT, true),
data_t(BACK, true),
data_t(BACK, false),
data_t(BACK, true),
data_t(DOWN, true),
data_t(RIGHT, false),
data_t(DOWN, true),
data_t(UP, false),
data_t(DOWN, false),
data_t(LEFT, true),
data_t(UP, true),
data_t(FRONT, false),
data_t(RIGHT, false),
data_t(BACK, true),
data_t(BACK, false),
data_t(LEFT, false),
data_t(DOWN, true),
data_t(DOWN, true),
data_t(UP, true),
data_t(BACK, true),
data_t(BACK, false),
data_t(RIGHT, false),
data_t(BACK, true),
data_t(BACK, false),
data_t(FRONT, true),
data_t(BACK, true),
data_t(LEFT, false),
data_t(BACK, false),
data_t(UP, false),
data_t(RIGHT, false),
data_t(RIGHT, true),
data_t(LEFT, true),
data_t(UP, false),
data_t(FRONT, true),
data_t(FRONT, true),
data_t(BACK, false),
data_t(FRONT, false),
data_t(RIGHT, true),
data_t(DOWN, true),
data_t(RIGHT, true),
data_t(BACK, true),
data_t(BACK, true),
data_t(BACK, true),
data_t(UP, true),
data_t(BACK, false),
data_t(LEFT, false),
data_t(UP, true),
data_t(RIGHT, true),
data_t(RIGHT, true),
data_t(LEFT, false),
data_t(FRONT, false),
data_t(UP, true),
data_t(RIGHT, true),
data_t(BACK, true),
data_t(DOWN, true),
data_t(FRONT, true),
data_t(UP, false),
data_t(LEFT, false),
data_t(BACK, true),
data_t(DOWN, true),
data_t(LEFT, false),
data_t(BACK, true),
data_t(DOWN, false),
data_t(BACK, true),
data_t(UP, true),
data_t(BACK, false),
data_t(DOWN, false),
};

// |1l|_IiIIl1|I|l1__ll|__I1_|lI_iI|1i11iIiiil|_i1l

int main() {
	Cube cube;

#if 0
	std::string str = "IiI_|l1|1l|I|1lII__|ll__Ii_Il|_1iIi11i1|l1i_|lii";
	//std::cin >> str;

	/*
0x4102353
0x31403014
0x22110223
0x42013253
0x4544535
0x55102541
	*/

	for (std::size_t i = 0; i < 6; ++i) {
		cube.sides[i] = 0;

		for (std::size_t j = 0; j < 8; ++j) {
			auto ch = '?';
			if (auto idx = i * 8 + j; str.size() > idx) {
				ch = str.at(idx);
			}

			uint32_t value = 6;
			if (ch == '|') {
				value = 0;
			}
			else if (ch == 'l') {
				value = 1;
			}
			else if (ch == '_') {
				value = 2;
			}
			else if (ch == 'I') {
				value = 3;
			}
			else if (ch == '1') {
				value = 4;
			}
			else if (ch == 'i') {
				value = 5;
			}

			cube.sides[i] |= (value << j * 4);
		}

		printf("0x%x\n", cube.sides[i]);
	}

	//assert(cube.sides[0] == 0x4102353);
	//assert(cube.sides[1] == 0x31403014);

	std::cout << (cube.solved() ? "true" : "false") << std::endl << cube << std::endl;
#endif

	cube.sides[0] = 0;
	cube.sides[1] = 0x11111111;
	cube.sides[2] = 0x22222222;
	cube.sides[3] = 0x33333333;
	cube.sides[4] = 0x44444444;
	cube.sides[5] = 0x55555555;

	std::cout << (cube.solved() ? "true" : "false") << std::endl << cube << std::endl;

	auto rot = [&](int type, bool reverse) {
		switch (type) {
		case UP:
			cube.up(reverse);
			return;
		case DOWN:
			cube.down(reverse);
			return;
		case LEFT:
			cube.left(reverse);
			return;
		case RIGHT:
			cube.right(reverse);
			return;
		case FRONT:
			cube.front(reverse);
			return;
		case BACK:
			cube.back(reverse);
			return;
		};

		__debugbreak();
		};

	for (int i = 100 - 1; i >= 0; --i) {
		auto& it = rotations[i];
		rot(it.type, !it.reverse);
	}

	std::cout << (cube.solved() ? "true" : "false") << std::endl << cube << std::endl;

	for (auto& it : rotations) {
		rot(it.type, it.reverse);
	}

	std::cout << (cube.solved() ? "true" : "false") << std::endl << cube << std::endl;

	return 0;
}
