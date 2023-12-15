#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP

#include <bitset>
#include <unordered_map>

#include "app.hpp"
#include "game/piece.hpp"

namespace app::game {

class Board {
public:
	explicit Board(bool empty = false);
	~Board()						= default;
	Board(const Board &)			= delete;
	Board &operator=(const Board &) = delete;

	void   init_board();
	void   flip_board();

	void   dump(bool merged = false) const;

private:
	typedef std::bitset<64>					bitboard;

	[[nodiscard]] bool						is_valid() const;

	void									dump_subboard(const PieceKind &kind) const;
	void									dump_merged_board() const;

	std::unordered_map<PieceKind, bitboard> boards;
	bool									is_flipped;
};

class Chess final : public Application {
public:
	Chess()							= default;
	~Chess() final					= default;
	Chess(const Chess &)			= delete;
	Chess &operator=(const Chess &) = delete;

	void   flip_board();

protected:
	void draw() const override;
	void handle_events(const SDL_Event &e) override;

private:
	Board board;
};

}  // namespace app::game

#endif	// CHESS_GAME_HPP
