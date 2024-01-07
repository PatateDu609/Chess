#ifndef CHESS_INCLUDE_GAME_GAME_HPP
#define CHESS_INCLUDE_GAME_GAME_HPP

#include <bitset>
#include <optional>
#include <unordered_map>
#include <vector>

#include "coord.hpp"
#include "game/piece.hpp"
#include "graphics/image.hpp"
#include "graphics/text.hpp"
#include "graphics/window.hpp"

namespace app::game {

class Board {
public:
	explicit Board(bool empty = false);

	~Board()														= default;
	Board(const Board &)											= delete;
	Board								  &operator=(const Board &) = delete;

	void								   init_board();
	void								   flip();
	[[nodiscard]] bool					   flipped() const;

	void								   dump(bool merged = false) const;

	[[nodiscard]] bool					   is_valid() const;

	[[nodiscard]] std::optional<PieceKind> at(size_t x, size_t y) const;
	[[nodiscard]] std::optional<PieceKind> at(const coord::Agnostic &c) const;

	void move_with_hint(const PieceKind &kind, const coord::Agnostic &origin, const coord::Agnostic &target);

private:
	typedef std::bitset<64>					bitboard;

	void									dump_subboard(const PieceKind &kind) const;
	void									dump_merged_board() const;

	[[nodiscard]] bool						check_static_move_validity(const PieceKind &kind,
							const coord::Agnostic										   &origin,
							const coord::Agnostic										   &target) const;

	std::unordered_map<PieceKind, bitboard> boards;
	bool									is_flipped;
	bool									base_game_pos;
};

}  // namespace app::game

#endif	// CHESS_INCLUDE_GAME_GAME_HPP
