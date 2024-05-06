#include "cube.hpp"

Cube::Cube()
{
	sides[0] = 0x00000000;
	sides[1] = 0x11111111;
	sides[2] = 0x22222222;
	sides[3] = 0x33333333;
	sides[4] = 0x44444444;
	sides[5] = 0x55555555;
}

Cube::Cube(uint32_t sides[6]) : Cube()
{
	for (size_t i = 0; i < 6; i++)
		this->sides[i] = sides[i];
}

Cube::Cube(const Cube& cube)
{
	for (size_t i = 0; i < 6; ++i)
		this->sides[i] = cube.sides[i];
}

Cube& Cube::operator=(const Cube& other)
{
	for (size_t i = 0; i < 6; ++i)
		sides[i] = other.sides[i];
	return *this;
}

uint32_t Cube::get_val_by_ij(size_t side, size_t row, size_t col) const
{
	if (row == 0)
		return (sides[side] & (0x0000000F << (col * 4))) >> (col * 4);

	if (row == 1) {
		if (col == 0)
			return (sides[side] & 0xF0000000) >> 28;
		if (col == 1)
			return static_cast<uint32_t>(side);
		if (col == 2)
			return (sides[side] & 0x0000F000) >> 12;
		return 0xFFFFFFFF;
	}

	if (row == 2)
		return (sides[side] & (0x0F000000 >> (col * 4))) >> (24 - col * 4);

	return 0xFFFFFFFF;
}

void Cube::up(bool reverse)
{
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

void Cube::down(bool reverse)
{
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

void Cube::left(bool reverse)
{
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

void Cube::right(bool reverse)
{
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

void Cube::front(bool reverse)
{
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

void Cube::back(bool reverse)
{
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

// clockwise rotation
void Cube::rotate(size_t side)
{
	uint32_t temp = (sides[side] & 0xFF000000) >> 24;
	sides[side] <<= 8;
	sides[side] |= temp;
}

uint32_t Cube::get_val(size_t side, size_t index)
{
	return (sides[side] & (0x0000000F << (index * 4))) >> (index * 4);
}

void Cube::set_val(size_t side, size_t index, uint32_t new_val)
{
	sides[side] &= ~(0x0000000F << (index * 4));
	sides[side] |= new_val << (index * 4);
}

uint32_t Cube::get_side(size_t index)
{
	return sides[index];
}

size_t Cube::get_corner_index(size_t i) {
	// Corner orientations and colors:
	// 0   1   2   3   4   5   6   7
	// UBL UBR UFR UFL F2L B2L B2R F2R
	size_t cur_corner[3];
	if (i == 0) {
		cur_corner[0] = get_val(0, 0); // UBL
		cur_corner[1] = get_val(4, 2);
		cur_corner[2] = get_val(1, 0);
	}
	else if (i == 1) {
		cur_corner[0] = get_val(0, 2); // UBR
		cur_corner[1] = get_val(4, 0);
		cur_corner[2] = get_val(3, 2);
	}
	else if (i == 2) {
		cur_corner[0] = get_val(0, 4); // UFR
		cur_corner[1] = get_val(2, 2);
		cur_corner[2] = get_val(3, 0);
	}
	else if (i == 3) {
		cur_corner[0] = get_val(0, 6); // UFL
		cur_corner[1] = get_val(2, 0);
		cur_corner[2] = get_val(1, 2);
	}
	else if (i == 4) {
		cur_corner[0] = get_val(5, 0); // F2L
		cur_corner[1] = get_val(2, 6);
		cur_corner[2] = get_val(1, 4);
	}
	else if (i == 5) {
		cur_corner[0] = get_val(5, 6); // B2L
		cur_corner[1] = get_val(4, 4);
		cur_corner[2] = get_val(1, 6);
	}
	else if (i == 6) {
		cur_corner[0] = get_val(5, 4); // B2R
		cur_corner[1] = get_val(4, 6);
		cur_corner[2] = get_val(3, 4);
	}
	else {
		cur_corner[0] = get_val(5, 2); // F2R
		cur_corner[1] = get_val(2, 4);
		cur_corner[2] = get_val(3, 6);
	}
	// sorting
	if (cur_corner[0] > cur_corner[1]) std::swap(cur_corner[0], cur_corner[1]);
	if (cur_corner[1] > cur_corner[2]) std::swap(cur_corner[1], cur_corner[2]);
	if (cur_corner[0] > cur_corner[1]) std::swap(cur_corner[0], cur_corner[1]);

	size_t result = -1;

	for (size_t i = 0; i < 8; ++i) {
		bool found = true;
		for (size_t j = 0; j < 3; ++j)
			if (cur_corner[j] != corners[i][j]) {
				found = false;
				break;
			}
		if (found) {
			result = i;
			break;
		}
	}
	return result;
}

void Cube::combo_move(std::list<Rotation>& sequence)
{
	for (Rotation x : sequence) {
		switch (x)
		{
		case Rotation::U:
			up();
			break;
		case Rotation::D:
			down();
			break;
		case Rotation::L:
			left();
			break;
		case Rotation::R:
			right();
			break;
		case Rotation::F:
			front();
			break;
		case Rotation::B:
			back();
			break;
		case Rotation::Up:
			up(true);
			break;
		case Rotation::Dp:
			down(true);
			break;
		case Rotation::Lp:
			left(true);
			break;
		case Rotation::Rp:
			right(true);
			break;
		case Rotation::Fp:
			front(true);
			break;
		case Rotation::Bp:
			back(true);
			break;
		case Rotation::U2:
			for (size_t i = 0; i < 2; ++i)
				up();
			break;
		case Rotation::D2:
			for (size_t i = 0; i < 2; ++i)
				down();
			break;
		case Rotation::L2:
			for (size_t i = 0; i < 2; ++i)
				left();
			break;
		case Rotation::R2:
			for (size_t i = 0; i < 2; ++i)
				right();
			break;
		case Rotation::F2:
			for (size_t i = 0; i < 2; ++i)
				front();
			break;
		case Rotation::B2:
			for (size_t i = 0; i < 2; ++i)
				back();
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Cube& cube)
{
	auto stringify = [](uint32_t v) {
		if (v == 0) return '|';
		if (v == 1) return 'l';
		if (v == 2) return '_';
		if (v == 3) return 'I';
		if (v == 4) return '1';
		if (v == 5) return 'i';
		return '?';
		};

	for (size_t i = 0; i < 3; ++i) {
		out << "    ";
		for (size_t j = 0; j < 3; ++j) {
			out << stringify(cube.get_val_by_ij(0, i, j));
		}
		out << '\n';
	}

	for (size_t i = 0; i < 3; ++i) {
		for (size_t side = 1; side <= 4; ++side) {
			for (size_t j = 0; j < 3; ++j) {
				out << stringify(cube.get_val_by_ij(side, i, j));
			}
			out << ' ';
		}
		out << '\n';
	}

	for (size_t i = 0; i < 3; ++i) {
		out << "    ";
		for (size_t j = 0; j < 3; ++j) {
			out << stringify(cube.get_val_by_ij(5, i, j));
		}
		out << '\n';
	}

	for (auto& side : cube.sides) {
		std::vector<std::uint32_t> side_bits = {};
		for (int i = 0; i < 8; ++i) {
			side_bits.push_back((side >> (i * 4)) & 0xF);
		}

		//std::reverse(side_bits.begin(), side_bits.end());

		for (auto v : side_bits) {
			printf("%c", stringify(v));
		}
	}

	printf("\n");
	return out;
}

bool Cube::on_group(size_t group)
{
	if (group == 1)
		return on_group1();

	if (group == 2)
		return on_group2();

	if (group == 3)
		return on_group3();

	if (group == 4)
		return solved();

	return true;
}

// state wherein all 12 edge pieces are correctly oriented
bool Cube::on_group1()
{
	uint32_t val;   // considered side of element
	uint32_t round; // its adjacent part
	for (size_t i = 1; i <= 4; ++i) {

		// checking U-layer
		val = get_val(0, 2 * i - 1);
		if (val == 1 || val == 3)
			return false;
		if (val == 2 || val == 4) {
			round = get_val(5 - i, 1);
			if (round == 0 || round == 5)
				return false;
		}

		// checking D-layer
		val = get_val(5, 2 * i - 1);
		if (val == 1 || val == 3)
			return false;
		if (val == 2 || val == 4) {
			round = get_val((i + 1) % 5, 5);
			if (round == 0 || round == 5)
				return false;
		}
	}

	// checking slice between UP and DOWN 
	val = get_val(2, 3);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(3, 7);
		if (round == 0 || round == 5)
			return false;
	}
	val = get_val(2, 7);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(1, 3);
		if (round == 0 || round == 5)
			return false;
	}
	val = get_val(4, 3);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(1, 7);
		if (round == 0 || round == 5)
			return false;
	}
	val = get_val(4, 7);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(3, 3);
		if (round == 0 || round == 5)
			return false;
	}
	return true;
}

// 1) state such that all corners are correctly oriented
// 2) four of the edges are moved to the correct slice:
//    (the layer between the up and down faces)
bool Cube::on_group2()
{
	for (size_t i = 0; i < 4; ++i)
		for (size_t j = 0; j <= 5; j += 5)
			if (get_val(j, i * 2) != 0 && get_val(j, i * 2) != 5)
				return false;


	for (size_t i = 1; i <= 4; ++i) {
		for (size_t j = 3; j <= 7; j += 4) {
			if (get_val(i, j) == 0 || get_val(i, j) == 5)
				return false;
		}
	}
	return true;
}

bool Cube::on_group3()
{
	size_t c_ind;

	// corners on their orbits
	c_ind = get_corner_index(0);
	if (c_ind != 0 && c_ind != 2)
		return false;
	c_ind = get_corner_index(2);
	if (c_ind != 0 && c_ind != 2)
		return false;

	c_ind = get_corner_index(1);
	if (c_ind != 1 && c_ind != 3)
		return false;
	c_ind = get_corner_index(3);
	if (c_ind != 1 && c_ind != 3)
		return false;

	c_ind = get_corner_index(4);
	if (c_ind != 4 && c_ind != 6)
		return false;
	c_ind = get_corner_index(6);
	if (c_ind != 4 && c_ind != 6)
		return false;

	// no LEFT or RIGHT colors on slice between LEFT and RIGHT
	for (size_t i = 0; i < 6; ++i) {
		if (i == 1 || i == 3)
			continue;
		for (size_t j = 1; j <= 5; j += 4)
			if (get_val(i, j) == 1 || get_val(i, j) == 3)
				return false;
	}

	return check_parity();
}

bool Cube::solved()
{
	return  sides[0] == 0x00000000 &&
		sides[1] == 0x11111111 &&
		sides[2] == 0x22222222 &&
		sides[3] == 0x33333333 &&
		sides[4] == 0x44444444 &&
		sides[5] == 0x55555555;
}

bool Cube::check_parity()
{
	// Corner orientations and colors:
	// 0   1   2   3   4   5   6   7
	// UBL UBR UFR UFL F2L B2L B2R F2R
	uint8_t parity = 0;

	for (size_t i = 0; i < 8; i++)
		for (size_t j = i + 1; j < 8; j++)
			parity ^= (get_corner_index(i) < get_corner_index(j));

	return parity == 0;
}
