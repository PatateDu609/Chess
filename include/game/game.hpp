#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP

#include <bitset>
#include <optional>
#include <unordered_map>
#include <vector>

#include "app.hpp"
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

	void							 full_dump() const;

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
	explicit Board(graphics::window::Window &window, bool empty = false);

	~Board()									= default;
	Board(const Board &)						= delete;
	Board			  &operator=(const Board &) = delete;

	void			   init_board(bool flip = false);
	void			   flip();

	void			   dump(bool merged = false) const;

	void			   select(size_t x, size_t y);
	void			   drop_selected(size_t x, size_t y);
	[[nodiscard]] bool has_selected() const;
	void			   move_pointer_piece(int x, int y);

	void			   update();
	void			   draw() const;

private:
	typedef std::bitset<64>										   bitboard;
	typedef std::unordered_map<PieceKind, graphics::ImageRenderer> PieceRenderers;

	struct PreRenderedBoardText {
		std::vector<graphics::TextRenderer> numbers;
		std::vector<graphics::TextRenderer> letters;
	};

	struct SelectedPiece {
		Coord	  coord;
		PieceKind kind;
		SDL_Rect  rect;

		ssize_t	  diff_x;
		ssize_t	  diff_y;
	};

	void									draw_pieces() const;
	void									draw_chessboard() const;
	void									draw_selected() const;

	[[nodiscard]] bool						is_valid() const;

	void									dump_subboard(const PieceKind &kind) const;
	void									dump_merged_board() const;

	void									check_pre_rendered(const std::shared_ptr<SDL_Renderer> &renderer);
	void									init_piece_renderers();

	[[nodiscard]] graphics::window::Coord	gen_sprite_coord(size_t x, size_t y) const;
	[[nodiscard]] size_t					get_piece_size() const;

	graphics::window::Window			   &win;

	std::unordered_map<PieceKind, bitboard> boards;
	const int								case_size;
	std::optional<SelectedPiece>			selected;

	PieceRenderers							piece_renderers;
	bool									is_flipped;
	bool									base_game_pos;

	std::pair<std::shared_ptr<SDL_Renderer>, PreRenderedBoardText> preRenderedBoardText;
};

class Chess final : public Application {
public:
	Chess()							= delete;
	~Chess() final					= default;
	Chess(const Chess &)			= delete;
	Chess &operator=(const Chess &) = delete;

	explicit Chess(graphics::window::Window &window);

protected:
	void draw() const override;
	void update() override;
	void handle_events(const SDL_Event &e) override;

private:
	Board					  board;
	graphics::window::Window &win;
};

}  // namespace app::game

#endif	// CHESS_GAME_HPP
