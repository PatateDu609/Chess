#include "game/coord.hpp"

#include <sstream>

namespace app::game::coord {

InvalidCoordException::InvalidCoordException(size_t x, size_t y) {
	std::ostringstream oss;

	oss << "invalid coord: (" << x << ", " << y << ")";

	const char *f = ":";
	if (x > 8) {
		oss << f << " x must be < 8";
		f = ",";
	}

	if (y > 8) {
		oss << f << " y must be < 8";
	}

	msg = oss.str();
}

InvalidCoordException::InvalidCoordException(const std::string &algebraic_form) {
	std::ostringstream oss;
	oss << "invalid algebraic form: " << algebraic_form;

	const char *f	   = ":";

	char		letter = static_cast<char>(tolower(algebraic_form[0]));
	if (letter < 'a' || 'h' < letter) {
		oss << f << " first char should be a letter in [a, h]";
		f = ",";
	}

	char nb = algebraic_form[1];
	if (nb < '1' || '8' < nb) {
		oss << f << "second char should be a number in [1, 8]";
	}

	msg = oss.str();
}

const char *InvalidCoordException::what() const noexcept {
	return msg.c_str();
}

// Agnostic class

Agnostic::Agnostic(uint8_t cx, uint8_t cy)
	: x(cx),
	  y(cy) {
	assert_validity();
}

bool Agnostic::is_valid() const {
	return x < 8 && y < 8;
}

void Agnostic::assert_validity() const noexcept(false) {
	if (!is_valid()) {
		throw InvalidCoordException(x, y);
	}
}

bool Agnostic::operator==(const app::game::coord::Agnostic &b) const {
	return x == b.x && y == b.y;
}

bool Agnostic::operator!=(const app::game::coord::Agnostic &b) const {
	return !(*this == b);
}

// Notation Class

Notation::Notation(bool board_flipped, uint8_t x, uint8_t y)
	: _board_flipped(board_flipped),
	  _x(x),
	  _y(board_flipped ? y : 7 - y) {
	if (!is_valid(x, y)) {
		throw InvalidCoordException(x, y);
	}

	update_algebraic();
}

Notation::Notation(bool board_flipped, std::string algebraic_form)
	: _algebraic(std::move(algebraic_form)),
	  _x(0),
	  _y(0),
	  _board_flipped(board_flipped) {
	if (!is_valid(_algebraic)) {
		throw InvalidCoordException(algebraic_form);
	}

	update_coord_couple();
}

const std::string &Notation::algebraic() const {
	return _algebraic;
}

void Notation::algebraic(std::string val) {
	_algebraic = std::move(val);
}

uint8_t Notation::x() const {
	return _x;
}

void Notation::x(uint8_t val) {
	_x = val;
}

uint8_t Notation::y() const {
	return _y;
}

void Notation::y(uint8_t val) {
	_y = val;
}

std::string Notation::to_string(bool full) const {
	std::ostringstream oss;

	oss << algebraic() << " (x = " << static_cast<int>(x()) << ", y = " << static_cast<int>(y()) << ")";
	return oss.str();
}

void Notation::update_algebraic() {
	const static std::string letters = "abcdefgh";

	_algebraic.assign(2, ' ');
	_algebraic[0] = letters[x()];
	_algebraic[1] = static_cast<char>('1' + y());
}

void Notation::update_coord_couple() {
	x(tolower(_algebraic[0]) - 'a');
	y(_algebraic[1] - '1');
}

bool Notation::is_valid(uint8_t x, uint8_t y) {
	return x < 8 && y < 8;
}

bool Notation::is_valid(std::string algebraic) {
	char letter = static_cast<char>(tolower(algebraic[0]));
	char nb		= algebraic[1];

	return ('a' <= letter && letter <= 'h') && ('1' <= nb && nb <= '8');
}

bool Notation::operator==(const Notation &other) const {
	return algebraic() == other.algebraic();
}

bool Notation::operator!=(const Notation &other) const {
	return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Notation &coord) {
	os << coord.algebraic() << " (x = " << static_cast<int>(coord.x())
	   << ", y = " << static_cast<int>(coord.y()) << "), board flipped ? ";
	os << std::boolalpha << coord._board_flipped << std::noboolalpha;

	return os;
}

}  // namespace app::game::coord

std::ostream &operator<<(std::ostream &os, const app::game::coord::Agnostic &coord) {
	return os << "(x = " << static_cast<int>(coord.x) << ", y = " << static_cast<int>(coord.y) << ")";
}
