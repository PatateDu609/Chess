#include "game/piece.hpp"

namespace app::game {

PieceKind::PieceKind(std::string name, bool is_white, std::string algebraic_name)
	: _name(std::move(name)),
	  _is_white(is_white),
	  _algebraic_name(std::move(algebraic_name)) {
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

const PieceKind				 PieceKind::BLACK_PAWN	 = PieceKind("pawn", false, "");
const PieceKind				 PieceKind::BLACK_KNIGHT = PieceKind("knight", false, "n");
const PieceKind				 PieceKind::BLACK_BISHOP = PieceKind("bishop", false, "b");
const PieceKind				 PieceKind::BLACK_ROOK	 = PieceKind("rook", false, "r");
const PieceKind				 PieceKind::BLACK_QUEEN	 = PieceKind("queen", false, "q");
const PieceKind				 PieceKind::BLACK_KING	 = PieceKind("king", false, "k");

const PieceKind				 PieceKind::WHITE_PAWN	 = PieceKind("pawn", true, "");
const PieceKind				 PieceKind::WHITE_KNIGHT = PieceKind("knight", true, "n");
const PieceKind				 PieceKind::WHITE_BISHOP = PieceKind("bishop", true, "b");
const PieceKind				 PieceKind::WHITE_ROOK	 = PieceKind("rook", true, "r");
const PieceKind				 PieceKind::WHITE_QUEEN	 = PieceKind("queen", true, "q");
const PieceKind				 PieceKind::WHITE_KING	 = PieceKind("king", true, "k");

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
