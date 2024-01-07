#include "graphics/game.hpp"

#include <iostream>

namespace graphics::game {

Board::Board(graphics::window::Window &window, bool empty)
	: win(window),
	  case_size(static_cast<int>(std::min(win.size().first / 8, win.size().second / 8))),
	  board(empty) {
	init_piece_renderers();
}

void Board::init_piece_renderers() {
	piece_renderers.clear();
	piece_renderers.reserve(PieceKind::ALL_PIECE_KINDS.size());

	using graphics::ImageRenderer;

	const size_t size = get_piece_size();
	for (const auto &kind : PieceKind::ALL_PIECE_KINDS) {
		ImageRenderer renderer(kind.get_sprite_path(), size, size);

		piece_renderers.emplace(kind, std::move(renderer));
	}
}

size_t Board::get_piece_size() const {
	return static_cast<size_t>(case_size * 0.8);
}

void Board::update() {
	auto weak_renderer = win.get_renderer();

	if (auto renderer = weak_renderer.lock()) {
		check_pre_rendered(renderer);
	}
}

void Board::draw() const {
	draw_chessboard();
	draw_pieces();
	draw_selected();
}

void Board::draw_chessboard() const {
	using graphics::Color;

	auto  weak_renderer = win.get_renderer();
	Color col;

	if (auto renderer = weak_renderer.lock()) {
		for (size_t y = 0; y < 8; y++) {
			bool y_even = y % 2 == 0;

			for (size_t x = 0; x < 8; x++) {
				bool x_even = x % 2 == 0;

				if (y_even) {
					col = x_even ? Color::LIGHT_SQUARE : Color::DARK_SQUARE;
				} else {
					col = x_even ? Color::DARK_SQUARE : Color::LIGHT_SQUARE;
				}

				SDL_Rect rect;
				rect.w = rect.h = case_size;
				rect.x			= static_cast<int>(x) * case_size;
				rect.y			= static_cast<int>(y) * case_size;

				SDL_SetRenderDrawColor(renderer.get(), col.r(), col.g(), col.b(), col.a());
				SDL_RenderFillRect(renderer.get(), &rect);
			}
		}

		for (size_t i = 0; i < 8; i++) {
			preRenderedBoardText.second.numbers[i].render();
			preRenderedBoardText.second.letters[i].render();
		}
	} else {
		throw std::runtime_error("couldn't get renderer: weak ptr expired");
	}
}

void Board::draw_pieces() const {
	if (!board.is_valid()) {
		std::cerr << "board not valid, can't draw pieces" << std::endl;
		return;
	}

	auto weak_renderer = win.get_renderer();

	if (auto renderer = weak_renderer.lock()) {
		size_t sx, sy;
		if (has_selected()) {
			sx = selected->coord.x;
			sy = selected->coord.y;
		}

		for (size_t y = 0; y < 8; y++) {
			for (size_t x = 0; x < 8; x++) {
				if (selected.has_value() && x == sx && y == sy) {
					continue;
				}

				auto kind = board.at(x, y);
				if (!kind.has_value()) {
					continue;
				}

				auto		  piece_renderer = piece_renderers.at(*kind);

				window::Coord tex_coord{};

				if (board.flipped()) {
					tex_coord = gen_sprite_coord(7 - x, 7 - y);
				} else {
					tex_coord = gen_sprite_coord(x, y);
				}

				piece_renderer.set_coord(tex_coord.x, tex_coord.y);
				piece_renderer.render(renderer);
			}
		}
	}
}

void Board::draw_selected() const {
	using std::max;

	if (!selected) return;

	auto   win_size = win.size();

	size_t x, y;
	if (auto renderer = win.get_renderer().lock()) {
		graphics::ImageRenderer img_renderer = piece_renderers.at(selected->kind);

		x									 = max(0, selected->rect.x);
		y									 = max(0, selected->rect.y);

		if (selected->rect.x + selected->rect.w > win_size.first) x = win_size.first - selected->rect.w;
		if (selected->rect.y + selected->rect.h > win_size.second) y = win_size.first - selected->rect.h;

		img_renderer.set_coord(x, y);
		img_renderer.set_size(selected->rect.w, selected->rect.h);
		img_renderer.render(renderer);
	} else {
		throw std::runtime_error("couldn't lock renderer");
	}
}

void Board::check_pre_rendered(const std::shared_ptr<SDL_Renderer> &renderer) {
	if (renderer == preRenderedBoardText.first) {
		return;
	}

	if (renderer != preRenderedBoardText.first) {
		preRenderedBoardText.second.numbers.clear();
		preRenderedBoardText.second.letters.clear();
	}

	using graphics::Color;

	preRenderedBoardText.first		  = renderer;
	PreRenderedBoardText &preRendered = preRenderedBoardText.second;
	const char			 *letters_str = board.flipped() ? "hgfedcba" : "abcdefgh";

	preRendered.numbers.reserve(8);
	preRendered.letters.reserve(8);

	for (size_t i = 0; i < 8; i++) {
		// Numbers initialization

		preRendered.numbers.emplace_back(renderer, "Segoe UI bold.ttf", 18);

		preRendered.numbers.back().set_coord(static_cast<size_t>(.06 * case_size),
			static_cast<size_t>((static_cast<double>(i) + 0.04) * case_size));
		preRendered.numbers.back().set_content(std::to_string(board.flipped() ? i + 1 : 8 - i));
		preRendered.numbers.back().set_color(i % 2 ? Color::LIGHT_SQUARE : Color::DARK_SQUARE);

		preRendered.numbers.back().reload_text();

		// Letters initialization

		preRendered.letters.emplace_back(renderer, "Segoe UI bold.ttf", 18);
		preRendered.letters.back().set_coord(static_cast<size_t>((static_cast<double>(i) + 0.8) * case_size),
			static_cast<size_t>(7.72 * case_size));
		preRendered.letters.back().set_content(std::string(1, letters_str[i]));
		preRendered.letters.back().set_color(i % 2 ? Color::DARK_SQUARE : Color::LIGHT_SQUARE);

		preRendered.letters.back().reload_text();
	}
}

void Board::flip() {
	preRenderedBoardText.first = nullptr;
	board.flip();
}

void Board::select(size_t x, size_t y) {
	using Coord = app::game::coord::Agnostic;

	Coord c(x / case_size, y / case_size);
	std::cout << "board coordinates: " << c << std::endl;

	auto piece_size = static_cast<int>(get_piece_size());

	auto kind		= board.at(c);
	if (!kind.has_value()) {
		return;
	}

	selected = SelectedPiece{
		.coord = c,
		.kind  = *kind,
		.moved = false,
	};

	auto sprite_coord = gen_sprite_coord(c.x, c.y);

	selected->rect.x  = static_cast<int>(sprite_coord.x);
	selected->rect.y  = static_cast<int>(sprite_coord.y);
	selected->rect.w  = piece_size;
	selected->rect.h  = piece_size;

	selected->diff_x  = static_cast<ssize_t>(x - selected->rect.x);
	selected->diff_y  = static_cast<ssize_t>(y - selected->rect.y);
}

void Board::drop_selected(size_t x, size_t y) {
	using Coord = app::game::coord::Agnostic;

	if (!has_selected()) return;

	if (!selected->moved) {
		std::cout << selected->coord << " not moved\n";
		selected.reset();
		return;
	}

	Coord target(x / case_size, y / case_size);
	board.move_with_hint(selected->kind, selected->coord, target);

	selected.reset();
}

bool Board::has_selected() const {
	return selected.has_value();
}

void Board::move_pointer_piece(int x, int y) {
	using Coord = app::game::coord::Agnostic;
	using graphics::ImageRenderer;

	if (!has_selected()) return;

	x				 = std::max(x, 0);
	y				 = std::max(y, 0);

	selected->rect.x = static_cast<int>(x - selected->diff_x);
	selected->rect.y = static_cast<int>(y - selected->diff_y);

	Coord c(x / case_size, y / case_size);
	if (c != selected->coord) {
		selected->moved = true;
	}
}

graphics::window::Coord Board::gen_sprite_coord(size_t x, size_t y) const {
	const double shift = case_size * 0.1;
	return {
		.x = static_cast<size_t>((static_cast<double>(x) * case_size) + shift),
		.y = static_cast<size_t>((static_cast<double>(y) * case_size) + shift),
	};
}

Chess::Chess(graphics::window::Window &window)
	: win(window),
	  board(window, false) {
}

void Chess::update() {
	board.update();
}

void Chess::draw() const {
	board.draw();
}

void Chess::handle_events(const SDL_Event &e) {
	switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					win.quit();
					break;

				case SDLK_f:
					board.flip();
					break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (e.button.button) {
				case SDL_BUTTON_LEFT:
					board.select(e.button.x, e.button.y);
					break;
			}
			break;
		case SDL_MOUSEMOTION:
			if (board.has_selected()) {
				board.move_pointer_piece(e.motion.x, e.motion.y);
			}

			break;
		case SDL_MOUSEBUTTONUP:
			switch (e.button.button) {
				case SDL_BUTTON_LEFT:
					board.drop_selected(e.button.x, e.button.y);
					break;
			}
			break;
	}
}

}  // namespace graphics::game
