#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP

#include <bitset>
#include <unordered_map>
#include <vector>

#include "app.hpp"
#include "game/piece.hpp"
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

	Coord(uint8_t x, uint8_t y);
	explicit Coord(std::string algebraic_form);

	Coord(const Coord &)									  = default;
	Coord							&operator=(const Coord &) = default;

	[[nodiscard]] const std::string &algebraic() const;
	void							 algebraic(std::string val);

	[[nodiscard]] uint8_t			 x() const;
	void							 x(uint8_t val);

	[[nodiscard]] uint8_t			 y() const;
	void							 y(uint8_t val);

	void							 full_dump() const;

private:
	void		update_algebraic();
	void		update_coord_couple();

	static bool is_valid(uint8_t x, uint8_t y);
	static bool is_valid(std::string algebraic);

	std::string _algebraic;
	uint8_t		_x;
	uint8_t		_y;
};

class Board {
public:
	explicit Board(bool empty = false);
	~Board()									= default;
	Board(const Board &)						= delete;
	Board			  &operator=(const Board &) = delete;

	void			   init_board();
	void			   flip_board();
	[[nodiscard]] bool flipped() const;

	void			   dump(bool merged = false) const;

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
	Chess()							= delete;
	~Chess() final					= default;
	Chess(const Chess &)			= delete;
	Chess &operator=(const Chess &) = delete;

	explicit Chess(graphics::window::Window &window);

	void flip_board();

protected:
	void draw() const override;
	void update() override;
	void handle_events(const SDL_Event &e) override;

private:
	Board					  board;
	graphics::window::Window &win;

	const int				  case_size;

	void					  check_pre_rendered(const std::shared_ptr<SDL_Renderer> &renderer);

	struct PreRenderedBoardText {
		std::vector<graphics::TextRenderer> numbers;
		std::vector<graphics::TextRenderer> letters;
	};

	std::pair<std::shared_ptr<SDL_Renderer>, PreRenderedBoardText> preRenderedBoardText;
};

}  // namespace app::game

std::ostream &operator<<(std::ostream &os, const app::game::Coord &coord);

#endif	// CHESS_GAME_HPP
