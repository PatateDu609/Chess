#ifndef CHESS_INCLUDE_GAME_HPP
#define CHESS_INCLUDE_GAME_HPP

#include <exception>
#include <string>

namespace app::game::coord {

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

struct Agnostic final {
public:
	Agnostic(uint8_t cx, uint8_t cy);

	[[nodiscard]] bool is_valid() const;
	void			   assert_validity() const noexcept(false);

	uint8_t			   x;
	uint8_t			   y;

	bool			   operator==(const Agnostic &b) const;
	bool			   operator!=(const Agnostic &b) const;
};

struct Notation final {
public:
	Notation(bool board_flipped, uint8_t x, uint8_t y);
	explicit Notation(bool board_flipped, std::string algebraic_form);

	Notation(const Notation &)									 = default;
	Notation						&operator=(const Notation &) = default;

	[[nodiscard]] const std::string &algebraic() const;
	void							 algebraic(std::string val);

	[[nodiscard]] uint8_t			 x() const;
	void							 x(uint8_t val);

	[[nodiscard]] uint8_t			 y() const;
	void							 y(uint8_t val);

	[[nodiscard]] std::string		 to_string(bool full) const;

	bool							 operator==(const Notation &other) const;
	bool							 operator!=(const Notation &other) const;

private:
	void				 update_algebraic();
	void				 update_coord_couple();

	static bool			 is_valid(uint8_t x, uint8_t y);
	static bool			 is_valid(std::string algebraic);

	std::string			 _algebraic;
	uint8_t				 _x;
	uint8_t				 _y;
	bool				 _board_flipped;

	friend std::ostream &operator<<(std::ostream &os, const app::game::coord::Notation &coord);
};

}  // namespace app::game::coord

std::ostream &operator<<(std::ostream &os, const app::game::coord::Agnostic &coord);

#endif	// CHESS_INCLUDE_GAME_HPP
