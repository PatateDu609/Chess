#include "game/game.hpp"

#include <SDL_ttf.h>

#include <bit>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "graphics/text.hpp"

namespace app::game {

Coord::InvalidCoordException::InvalidCoordException(size_t x, size_t y) {
	std::ostringstream oss;
	oss << "invalid coord: (" << x << ", " << y << ")";

	const char *f = ":";
	if (x > 8) {
		oss << f << " x must be < 8";
		f = ",";
	}

	if (y > 8) {
		oss << f << " y must be < 8";
	}

	msg = oss.str();
}

Coord::InvalidCoordException::InvalidCoordException(const std::string &algebraic_form) {
	std::ostringstream oss;
	oss << "invalid algebraic form: " << algebraic_form;

	const char *f	   = ":";

	char		letter = static_cast<char>(tolower(algebraic_form[0]));
	if (letter < 'a' || 'h' < letter) {
		oss << f << " first char should be a letter in [a, h]";
		f = ",";
	}

	char nb = algebraic_form[1];
	if (nb < '1' || '8' < nb) {
		oss << f << "second char should be a number in [1, 8]";
	}

	msg = oss.str();
}

const char *Coord::InvalidCoordException::what() const noexcept {
	return msg.c_str();
}

Coord::Coord(bool board_flipped, uint8_t x, uint8_t y)
	: _board_flipped(board_flipped),
	  _x(x),
	  _y(y) {
	if (!is_valid(x, y)) {
		throw InvalidCoordException(x, y);
	}

	update_algebraic();
}

Coord::Coord(bool board_flipped, std::string algebraic_form)
	: _algebraic(std::move(algebraic_form)),
	  _x(0),
	  _y(0),
	  _board_flipped(board_flipped) {
	if (!is_valid(_algebraic)) {
		throw InvalidCoordException(algebraic_form);
	}

	update_coord_couple();
}

const std::string &Coord::algebraic() const {
	return _algebraic;
}

void Coord::algebraic(std::string val) {
	_algebraic = std::move(val);
}

uint8_t Coord::x() const {
	return _x;
}

void Coord::x(uint8_t val) {
	_x = val;
}

uint8_t Coord::y() const {
	return _y;
}

void Coord::y(uint8_t val) {
	_y = val;
}

void Coord::full_dump() const {
	std::cout << algebraic() << " (x = " << static_cast<int>(x()) << ", y = " << static_cast<int>(y()) << ")";
}

void Coord::update_algebraic() {
	const static std::string letters = "abcdefgh";

	_algebraic.assign(2, ' ');
	_algebraic[0] = letters[x()];
	_algebraic[1] = static_cast<char>('1' + y());
}

void Coord::update_coord_couple() {
	x(tolower(_algebraic[0]) - 'a');
	y(_algebraic[1] - '1');
}

bool Coord::is_valid(uint8_t x, uint8_t y) {
	return x < 8 && y < 8;
}

bool Coord::is_valid(std::string algebraic) {
	char letter = static_cast<char>(tolower(algebraic[0]));
	char nb		= algebraic[1];

	return ('a' <= letter && letter <= 'h') && ('1' <= nb && nb <= '8');
}
std::ostream &operator<<(std::ostream &os, const Coord &coord) {
	os << coord.algebraic() << " (x = " << static_cast<int>(coord.x())
	   << ", y = " << static_cast<int>(coord.y()) << "), board flipped ? ";
	os << std::boolalpha << coord._board_flipped << std::noboolalpha;

	return os;
}

size_t Board::get_piece_size() const {
	return static_cast<size_t>(case_size * 0.8);
}

Board::Board(graphics::window::Window &window, bool empty)
	: win(window),
	  base_game_pos(false),
	  is_flipped(false),
	  case_size(static_cast<int>(std::min(win.size().first / 8, win.size().second / 8))) {
	if (!empty) init_board(false);

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

void Board::init_board(bool flip) {
	for (auto &pair : boards) {
		pair.second = 0;
	}

	is_flipped	  = flip;
	base_game_pos = true;

	uint64_t				 tmp_board;
	static constexpr uint8_t pawn_setup			= 0b11111111;
	static constexpr uint8_t rook_setup			= 0b10000001;
	static constexpr uint8_t knight_setup		= 0b01000010;
	static constexpr uint8_t bishop_setup		= 0b00100100;
	const uint8_t			 king_setup			= is_flipped ? 0b00001000 : 0b00010000;
	const uint8_t			 queen_setup		= is_flipped ? 0b00010000 : 0b00001000;

	const int				 white_pawns_shift	= is_flipped ? 8 : 48;
	const int				 black_pawns_shift	= is_flipped ? 48 : 8;

	const int				 white_pieces_shift = white_pawns_shift == 48 ? 56 : 0;
	const int				 black_pieces_shift = black_pawns_shift == 48 ? 56 : 0;

	tmp_board									= static_cast<uint64_t>(pawn_setup) << black_pawns_shift;
	boards[PieceKind::BLACK_PAWN]				= tmp_board;
	tmp_board									= static_cast<uint64_t>(pawn_setup) << white_pawns_shift;
	boards[PieceKind::WHITE_PAWN]				= tmp_board;

	tmp_board									= static_cast<uint64_t>(knight_setup) << black_pieces_shift;
	boards[PieceKind::BLACK_KNIGHT]				= tmp_board;
	tmp_board									= static_cast<uint64_t>(knight_setup) << white_pieces_shift;
	boards[PieceKind::WHITE_KNIGHT]				= tmp_board;

	tmp_board									= static_cast<uint64_t>(bishop_setup) << black_pieces_shift;
	boards[PieceKind::BLACK_BISHOP]				= tmp_board;
	tmp_board									= static_cast<uint64_t>(bishop_setup) << white_pieces_shift;
	boards[PieceKind::WHITE_BISHOP]				= tmp_board;

	tmp_board									= static_cast<uint64_t>(rook_setup) << black_pieces_shift;
	boards[PieceKind::BLACK_ROOK]				= tmp_board;
	tmp_board									= static_cast<uint64_t>(rook_setup) << white_pieces_shift;
	boards[PieceKind::WHITE_ROOK]				= tmp_board;

	boards[PieceKind::WHITE_QUEEN]				= static_cast<uint64_t>(queen_setup) << white_pieces_shift;
	boards[PieceKind::WHITE_KING]				= static_cast<uint64_t>(king_setup) << white_pieces_shift;

	boards[PieceKind::BLACK_QUEEN]				= static_cast<uint64_t>(queen_setup) << black_pieces_shift;
	boards[PieceKind::BLACK_KING]				= static_cast<uint64_t>(king_setup) << black_pieces_shift;
}

void Board::flip() {
	is_flipped				   = !is_flipped;
	preRenderedBoardText.first = nullptr;

	if (base_game_pos) {
		init_board(is_flipped);
	} else {
		for (auto &pair : boards) {
			pair.second = std::byteswap(pair.second.to_ullong());
		}
	}
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

graphics::window::Coord Board::gen_sprite_coord(size_t x, size_t y) const {
	const double shift = case_size * 0.1;
	return {
		.x = static_cast<size_t>((static_cast<double>(x) * case_size) + shift),
		.y = static_cast<size_t>((static_cast<double>(y) * case_size) + shift),
	};
}

void Board::draw_pieces() const {
	if (!is_valid()) {
		std::cerr << "board not valid, can't draw pieces" << std::endl;
		return;
	}

	auto weak_renderer = win.get_renderer();

	if (auto renderer = weak_renderer.lock()) {
		for (const auto &pair : boards) {
			auto board = pair.second;

			if (board.none()) continue;

			auto kind			= pair.first;
			auto piece_renderer = piece_renderers.at(kind);

			for (size_t i = 0; i < board.size(); i++) {
				if (!board[i]) continue;

				size_t x = i % 8;
				size_t y = i / 8;

				if (selected.has_value() && x == selected->coord.x() && y == selected->coord.y()) {
					continue;
				}

				auto tex_coord = gen_sprite_coord(x, y);
				piece_renderer.set_coord(tex_coord.x, tex_coord.y);
				piece_renderer.render(renderer);
			}
		}
	}
}

void Board::draw_selected() const {
	if (!selected) return;

	if (auto renderer = win.get_renderer().lock()) {
		graphics::ImageRenderer img_renderer = piece_renderers.at(selected->kind);

		img_renderer.set_coord(selected->rect.x, selected->rect.y);
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
	const char			 *letters_str = is_flipped ? "hgfedcba" : "abcdefgh";

	preRendered.numbers.reserve(8);
	preRendered.letters.reserve(8);

	for (size_t i = 0; i < 8; i++) {
		// Numbers initialization

		preRendered.numbers.emplace_back(renderer, "Segoe UI bold.ttf", 18);

		preRendered.numbers.back().set_coord(static_cast<size_t>(.06 * case_size),
			static_cast<size_t>((static_cast<double>(i) + 0.04) * case_size));
		preRendered.numbers.back().set_content(std::to_string(is_flipped ? i + 1 : 8 - i));
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

void Board::dump(bool merged) const {
	std::cout << "Board is " << (is_valid() ? "" : "NOT ") << "valid\n";

	if (merged) {
		dump_merged_board();
	} else {
		dump_subboard(PieceKind::WHITE_PAWN);
		dump_subboard(PieceKind::WHITE_ROOK);
		dump_subboard(PieceKind::WHITE_KNIGHT);
		dump_subboard(PieceKind::WHITE_BISHOP);
		dump_subboard(PieceKind::WHITE_KING);
		dump_subboard(PieceKind::WHITE_QUEEN);

		dump_subboard(PieceKind::BLACK_PAWN);
		dump_subboard(PieceKind::BLACK_ROOK);
		dump_subboard(PieceKind::BLACK_KNIGHT);
		dump_subboard(PieceKind::BLACK_BISHOP);
		dump_subboard(PieceKind::BLACK_KING);
		dump_subboard(PieceKind::BLACK_QUEEN);
	}
}

void Board::dump_merged_board() const {
	std::array<std::array<std::string, 8>, 8> board;
	int										  largest_case	 = 0;

	size_t									  board_line_idx = 0;
	auto dump_line = [this, &board_line_idx, &board, &largest_case](size_t y) {
		for (size_t x = 0; x < 8; x++) {
			std::vector<std::string> bCase{};
			bCase.reserve(1);

			for (const auto &p : boards) {
				bool c = p.second[y * 8 + x];

				if (c) {
					std::string name = p.first.get_algebraic_name();
					if (name.empty()) name = "p";
					if (!p.first.is_white())
						std::transform(name.begin(), name.end(), name.begin(), ::toupper);

					bCase.emplace_back(name);
				}
			}

			if (bCase.size() == 1) {
				board[board_line_idx][x] = bCase[0];
			} else if (!bCase.empty()) {
				std::ostringstream oss;
				auto			   begin = bCase.begin(), end = bCase.end();

				std::copy(begin, std::prev(end), std::ostream_iterator<std::string>(oss, ","));
				begin = std::prev(end);
				if (begin != end) {
					oss << *begin;
				}

				board[board_line_idx][x] = oss.str();
			}

			largest_case = std::max(static_cast<int>(board[board_line_idx][x].size()), largest_case);
		}
		board_line_idx++;
	};

	if (is_flipped) {
		for (size_t y = 8; y > 0; y--) {
			dump_line(y - 1);
		}
	} else {
		for (size_t y = 0; y < 8; y++) {
			dump_line(y);
		}
	}

	for (size_t y = 0; y < 8; y++) {
		for (size_t x = 0; x < 8; x++) {
			std::string str = board[y][x];

			if (str.empty()) str = ".";

			if (static_cast<int>(str.size()) == largest_case) {
				std::cout << str;
			} else {
				float side_sp = static_cast<float>(largest_case - str.size()) / 2.f;
				std::cout << std::string(std::floor(side_sp), ' ') << str
						  << std::string(std::floor(side_sp), ' ');
			}

			std::cout << (x < 7 ? " " : std::string(1 + (largest_case + 1) / 4, '\n'));
		}
	}
}

void Board::dump_subboard(const PieceKind &kind) const {
	bitboard	board	  = boards.at(kind);

	std::string piece_rep = kind.get_algebraic_name();
	if (piece_rep.empty()) piece_rep = "p";

	std::string color = kind.is_white() ? "white" : "black";
	std::string piece = kind.get_name();

	if (!kind.is_white()) std::transform(piece_rep.begin(), piece_rep.end(), piece_rep.begin(), ::toupper);

	std::cout << "Board with " << color << " " << piece << "\n";

	auto dump_line = [piece_rep, board](size_t y) {
		for (size_t x = 0; x < 8; x++) {
			bool c = board[y * 8 + x];

			if (c)
				std::cout << piece_rep;
			else
				std::cout << "_";
			std::cout << (x < 7 ? " " : "\n");
		}
	};

	if (is_flipped) {
		for (size_t y = 8; y > 0; y--) {
			dump_line(y - 1);
		}
	} else {
		for (size_t y = 0; y < 8; y++) {
			dump_line(y);
		}
	}
}

bool Board::is_valid() const {
	auto acc_fn = [](size_t a, decltype(boards)::const_reference b) {
		return a + b.second.count();
	};

	size_t	 ref = std::accumulate(boards.begin(), boards.end(), 0, acc_fn);

	bitboard final;

	for (const auto &p : boards) {
		final |= p.second;
	}

	return final.count() == ref;
}

void Board::select(size_t x, size_t y) {
	Coord c(is_flipped, x / case_size, y / case_size);
	std::cout << "board coordinates: " << c << std::endl;

	size_t bitboardCoord = c.y() * 8 + c.x();
	auto   piece_size	 = static_cast<int>(get_piece_size());
	for (const auto &pair : boards) {
		if (!pair.second[bitboardCoord]) {
			continue;
		}

		selected = SelectedPiece{
			.coord	   = c,
			.kind	   = pair.first,
			.win_coord = graphics::window::Coord{.x = x, .y = y},
		};

		auto sprite_coord = gen_sprite_coord(c.x(), c.y());

		selected->rect.x  = static_cast<int>(sprite_coord.x);
		selected->rect.y  = static_cast<int>(sprite_coord.y);
		selected->rect.w  = piece_size;
		selected->rect.h  = piece_size;

		selected->diff_x  = static_cast<ssize_t>(selected->win_coord.x - selected->rect.x);
		selected->diff_y  = static_cast<ssize_t>(selected->win_coord.y - selected->rect.y);

		break;
	}
}

void Board::unselect() {
	selected.reset();
}

bool Board::has_selected() const {
	return selected.has_value();
}

void Board::move_pointer_piece(size_t x, size_t y) {
	using graphics::ImageRenderer;

	if (!has_selected()) return;

	selected->rect.x = static_cast<int>(x - selected->diff_x);
	selected->rect.y = static_cast<int>(y - selected->diff_y);
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
					board.unselect();
					break;
			}
			break;
	}
}

}  // namespace app::game
