#ifndef CHESS_INCLUDE_GRAPHICS_HPP
#define CHESS_INCLUDE_GRAPHICS_HPP

#include <optional>

#include "app.hpp"
#include "game/game.hpp"
#include "game/piece.hpp"

namespace graphics::game {

class Board {
public:
	Board(graphics::window::Window &window, bool empty);

	void			   update();
	void			   draw() const;

	void			   flip();

	void			   select(size_t x, size_t y);
	[[nodiscard]] bool has_selected() const;
	void			   move_pointer_piece(int x, int y);
	void			   drop_selected(size_t x, size_t y);

private:
	using PieceKind = app::game::PieceKind;

	typedef std::unordered_map<app::game::PieceKind, graphics::ImageRenderer> PieceRenderers;

	struct PreRenderedBoardText {
		std::vector<graphics::TextRenderer> numbers;
		std::vector<graphics::TextRenderer> letters;
	};

	struct SelectedPiece {
		app::game::Coord coord;
		PieceKind		 kind;
		SDL_Rect		 rect;

		ssize_t			 diff_x;
		ssize_t			 diff_y;
		bool			 moved;
	};

	void								  draw_pieces() const;
	void								  draw_chessboard() const;
	void								  draw_selected() const;

	void								  check_pre_rendered(const std::shared_ptr<SDL_Renderer> &renderer);
	void								  init_piece_renderers();
	[[nodiscard]] graphics::window::Coord gen_sprite_coord(size_t x, size_t y) const;
	[[nodiscard]] size_t				  get_piece_size() const;

	graphics::window::Window			 &win;
	app::game::Board					  board;
	const int							  case_size;

	std::optional<SelectedPiece>		  selected;
	PieceRenderers						  piece_renderers;

	std::pair<std::shared_ptr<SDL_Renderer>, PreRenderedBoardText> preRenderedBoardText;
};

class Chess final : public app::Application {
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

}  // namespace graphics::game

#endif	// CHESS_INCLUDE_GRAPHICS_HPP
