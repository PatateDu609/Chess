#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include <string>

namespace app::game {

class PieceKind final {
public:
	static const PieceKind BLACK_PAWN;
	static const PieceKind BLACK_KNIGHT;
	static const PieceKind BLACK_BISHOP;
	static const PieceKind BLACK_ROOK;
	static const PieceKind BLACK_QUEEN;
	static const PieceKind BLACK_KING;

	static const PieceKind WHITE_PAWN;
	static const PieceKind WHITE_KNIGHT;
	static const PieceKind WHITE_BISHOP;
	static const PieceKind WHITE_ROOK;
	static const PieceKind WHITE_QUEEN;
	static const PieceKind WHITE_KING;

private:
	PieceKind(std::string name, bool is_white, std::string algebraic_name);

public:
	[[nodiscard]] std::string get_name() const;
	[[nodiscard]] bool		  is_white() const;
	[[nodiscard]] std::string get_algebraic_name() const;

	bool					  operator==(const PieceKind &other) const;

private:
	std::string _name;
	bool		_is_white;
	std::string _algebraic_name;

	friend struct std::hash<PieceKind>;
};

}  // namespace app::game

template <>
struct std::hash<app::game::PieceKind> {
	std::size_t operator()(const app::game::PieceKind &a) const {
		using std::hash;
		using std::size_t;
		using std::string;

		return (hash<string>()(a._name) ^ hash<string>()(a._algebraic_name) ^ hash<bool>()(a._is_white));
	}
};

#endif	// CHESS_PIECE_HPP
