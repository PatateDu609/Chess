#ifndef CHESS_INCLUDE_GAME_GAME_HPP
#define CHESS_INCLUDE_GAME_GAME_HPP

#include <bitset>
#include <optional>
#include <unordered_map>
#include <vector>

#include "game/piece.hpp"
#include "graphics/image.hpp"
#include "graphics/text.hpp"
#include "graphics/window.hpp"

namespace app::game {

struct Coord final {
public:
	class InvalidCoordException : public std::exception {
	public:
		explicit InvalidCoordException(const std::string &algebraic_form);
		InvalidCoordException(size_t x, size_t y);

		InvalidCoordException(const InvalidCoordException &)			= default;
		InvalidCoordException &operator=(const InvalidCoordException &) = default;

		~InvalidCoordException() override								= default;

		[[nodiscard]] const char *what() const noexcept override;

	private:
		std::string msg;
	};

	Coord(bool board_flipped, uint8_t x, uint8_t y);
	explicit Coord(bool board_flipped, std::string algebraic_form);

	Coord(const Coord &)									  = default;
	Coord							&operator=(const Coord &) = default;

	[[nodiscard]] const std::string &algebraic() const;
	void							 algebraic(std::string val);

	[[nodiscard]] uint8_t			 x() const;
	void							 x(uint8_t val);

	[[nodiscard]] uint8_t			 y() const;
	void							 y(uint8_t val);

	[[nodiscard]] std::string		 to_string(bool full) const;

	bool							 operator==(const Coord &other) const;
	bool							 operator!=(const Coord &other) const;

private:
	void				 update_algebraic();
	void				 update_coord_couple();

	static bool			 is_valid(uint8_t x, uint8_t y);
	static bool			 is_valid(std::string algebraic);

	std::string			 _algebraic;
	uint8_t				 _x;
	uint8_t				 _y;
	bool				 _board_flipped;

	friend std::ostream &operator<<(std::ostream &os, const app::game::Coord &coord);
};

class Board {
public:
	explicit Board(bool empty = false);

	~Board()														= default;
	Board(const Board &)											= delete;
	Board								  &operator=(const Board &) = delete;

	void								   init_board(bool flip = false);
	void								   flip();
	[[nodiscard]] bool					   flipped() const;

	void								   dump(bool merged = false) const;

	[[nodiscard]] bool					   is_valid() const;

	[[nodiscard]] std::optional<PieceKind> at(size_t x, size_t y) const;
	[[nodiscard]] std::optional<PieceKind> at(const Coord &c) const;

	void								   move_with_hint(const PieceKind &kind, const Coord &origin, const Coord &target);

private:
	typedef std::bitset<64>					bitboard;

	void									dump_subboard(const PieceKind &kind) const;
	void									dump_merged_board() const;

	std::unordered_map<PieceKind, bitboard> boards;
	bool									is_flipped;
	bool									base_game_pos;
};

}  // namespace app::game

#endif	// CHESS_GAME_HPP
