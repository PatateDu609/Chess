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

Board::Board(bool empty)
	: base_game_pos(false),
	  is_flipped(false) {
	if (!empty) init_board();
}

void Board::init_board() {
	for (auto &pair : boards) {
		pair.second = 0;
	}

	base_game_pos = true;

	uint64_t				 tmp_board;
	static constexpr uint8_t pawn_setup			= 0b11111111;
	static constexpr uint8_t rook_setup			= 0b10000001;
	static constexpr uint8_t knight_setup		= 0b01000010;
	static constexpr uint8_t bishop_setup		= 0b00100100;
	static constexpr uint8_t king_setup			= 0b00010000;
	static constexpr uint8_t queen_setup		= 0b00001000;

	static constexpr int	 white_pawns_shift	= 48;
	static constexpr int	 black_pawns_shift	= 8;

	static constexpr int	 white_pieces_shift = 56;
	static constexpr int	 black_pieces_shift = 0;

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

bool Board::flipped() const {
	return is_flipped;
}

void Board::flip() {
	is_flipped = !is_flipped;
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

std::optional<PieceKind> Board::at(size_t x, size_t y) const {
	return at(coord::Agnostic(x, y));
}

std::optional<PieceKind> Board::at(const coord::Agnostic &c) const {
	size_t bitboardCoord = c.y * 8 + c.x;

	for (const auto &pair : boards) {
		if (pair.second[bitboardCoord]) {
			return pair.first;
		}
	}

	return std::nullopt;
}

bool Board::check_static_move_validity(const PieceKind &kind,
	const coord::Agnostic							   &origin,
	const coord::Agnostic							   &target) const {
	coord::Notation o(is_flipped, origin.x, origin.y);
	coord::Notation t(is_flipped, target.x, target.y);

	if (!kind(o, t)) {
		std::cerr << "move " << kind.get_name() << " " << o.algebraic() << " to " << t.algebraic()
				  << " invalid\n";
		return false;
	}

	return true;
}

void Board::move_with_hint(const PieceKind &kind,
	const coord::Agnostic				   &origin,
	const coord::Agnostic				   &target) {
	size_t target_idx = target.y * 8 + target.x;

	for (auto pair : boards) {
		if (pair.second[target_idx]) return;
	}

	bitboard &board		 = boards.at(kind);

	size_t	  origin_idx = origin.y * 8 + origin.x;

	if (!board[origin_idx]) return;

	if (!check_static_move_validity(kind, origin, target)) {
		return;
	}

	board[origin_idx] = false;
	board[target_idx] = true;
}

}  // namespace app::game
