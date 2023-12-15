#include "game/piece.hpp"

namespace app::game {

PieceKind::PieceKind(std::string name, bool is_white, std::string algebraic_name)
	: _name(std::move(name)), _is_white(is_white), _algebraic_name(std::move(algebraic_name)) {}

std::string PieceKind::get_algebraic_name() const { return _algebraic_name; }

std::string PieceKind::get_name() const { return _name; }

bool		PieceKind::is_white() const { return _is_white; }

bool		PieceKind::operator==(const PieceKind& other) const {
	   return _name == other._name && _is_white == other._is_white && _algebraic_name == other._algebraic_name;
}

const PieceKind PieceKind::BLACK_PAWN	= PieceKind("pawn", false, "");
const PieceKind PieceKind::BLACK_KNIGHT = PieceKind("knight", false, "n");
const PieceKind PieceKind::BLACK_BISHOP = PieceKind("bishop", false, "b");
const PieceKind PieceKind::BLACK_ROOK	= PieceKind("rook", false, "r");
const PieceKind PieceKind::BLACK_QUEEN	= PieceKind("queen", false, "q");
const PieceKind PieceKind::BLACK_KING	= PieceKind("king", false, "k");

const PieceKind PieceKind::WHITE_PAWN	= PieceKind("pawn", true, "");
const PieceKind PieceKind::WHITE_KNIGHT = PieceKind("knight", true, "n");
const PieceKind PieceKind::WHITE_BISHOP = PieceKind("bishop", true, "b");
const PieceKind PieceKind::WHITE_ROOK	= PieceKind("rook", true, "r");
const PieceKind PieceKind::WHITE_QUEEN	= PieceKind("queen", true, "q");
const PieceKind PieceKind::WHITE_KING	= PieceKind("king", true, "k");

}  // namespace app::game
