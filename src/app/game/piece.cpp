#include "game/piece.hpp"

#include <utility>

#include "game/game.hpp"

using Coord = app::game::coord::Notation;

namespace app::game {

PieceKind::PieceKind(std::string name, bool is_white, std::string algebraic_name, MoveChecker checker)
	: _name(std::move(name)),
	  _is_white(is_white),
	  _algebraic_name(std::move(algebraic_name)),
	  _checker(std::move(checker)) {
}

std::string PieceKind::get_algebraic_name() const {
	return _algebraic_name;
}

std::string PieceKind::get_name() const {
	return _name;
}

bool PieceKind::is_white() const {
	return _is_white;
}

std::filesystem::path PieceKind::get_sprite_path() const {
	using std::filesystem::path;

	path final	= is_white() ? "light" : "dark";
	final	   /= get_name() + ".svg";

	return final;
}

bool PieceKind::operator==(const PieceKind& other) const {
	return _name == other._name && _is_white == other._is_white && _algebraic_name == other._algebraic_name;
}

bool PieceKind::operator()(const Coord& origin, const Coord& target) const {
	if (_checker) return _checker(origin, target);
	return true;
}

struct PawnStaticChecker {
	explicit PawnStaticChecker(bool is_white)
		: _is_white(is_white) {
	}

	bool operator()(const Coord& origin, const Coord& target) const {
		ssize_t dx = target.x() - origin.x();
		ssize_t dy = target.y() - origin.y();

		if (std::abs(dy) != 1 && std::abs(dy) != 2) return false;
		if (std::abs(dx) > 1) return false;
		if (dx == 1) return dy == 1;

		return (_is_white && dy > 0) || (!_is_white && 0 > dy);
	}

private:
	bool _is_white;
};

bool knight_static_checker(const Coord& origin, const Coord& target) {
	ssize_t dx = std::abs(target.x() - origin.x());
	ssize_t dy = std::abs(target.y() - origin.y());

	return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

bool bishop_static_checker(const Coord& origin, const Coord& target) {
	ssize_t dx = target.x() - origin.x();
	ssize_t dy = target.y() - origin.y();

	return std::abs(dx) == std::abs(dy);
}

bool rook_static_checker(const Coord& origin, const Coord& target) {
	ssize_t dx = target.x() - origin.x();
	ssize_t dy = target.y() - origin.y();

	return (dx == 0 && dy != 0) || (dx != 0 && dy == 0);
}

bool queen_static_checker(const Coord& origin, const Coord& target) {
	return rook_static_checker(origin, target) || bishop_static_checker(origin, target);
}

bool king_static_checker(const Coord& origin, const Coord& target) {
	ssize_t dx = target.x() - origin.x();
	ssize_t dy = target.y() - origin.y();

	return std::abs(dx) <= 1 && std::abs(dy) <= 1;
}

const PieceKind				 PieceKind::BLACK_PAWN	 = PieceKind("pawn", false, "", PawnStaticChecker(false));
const PieceKind				 PieceKind::BLACK_KNIGHT = PieceKind("knight", false, "n", knight_static_checker);
const PieceKind				 PieceKind::BLACK_BISHOP = PieceKind("bishop", false, "b", bishop_static_checker);
const PieceKind				 PieceKind::BLACK_ROOK	 = PieceKind("rook", false, "r", rook_static_checker);
const PieceKind				 PieceKind::BLACK_QUEEN	 = PieceKind("queen", false, "q", queen_static_checker);
const PieceKind				 PieceKind::BLACK_KING	 = PieceKind("king", false, "k", king_static_checker);

const PieceKind				 PieceKind::WHITE_PAWN	 = PieceKind("pawn", true, "", PawnStaticChecker(true));
const PieceKind				 PieceKind::WHITE_KNIGHT = PieceKind("knight", true, "n", knight_static_checker);
const PieceKind				 PieceKind::WHITE_BISHOP = PieceKind("bishop", true, "b", bishop_static_checker);
const PieceKind				 PieceKind::WHITE_ROOK	 = PieceKind("rook", true, "r", rook_static_checker);
const PieceKind				 PieceKind::WHITE_QUEEN	 = PieceKind("queen", true, "q", queen_static_checker);
const PieceKind				 PieceKind::WHITE_KING	 = PieceKind("king", true, "k", king_static_checker);

const std::vector<PieceKind> PieceKind::ALL_PIECE_KINDS{
	BLACK_PAWN,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_ROOK,
	BLACK_QUEEN,
	BLACK_KING,
	WHITE_PAWN,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_ROOK,
	WHITE_QUEEN,
	WHITE_KING,
};

}  // namespace app::game

std::ostream& operator<<(std::ostream& os, const app::game::PieceKind& kind) {
	os << (kind.is_white() ? "white" : "black") << " ";
	os << kind.get_name();
	return os;
}
