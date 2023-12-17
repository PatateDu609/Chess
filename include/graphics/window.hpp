#ifndef CHESS_WINDOW_HPP
#define CHESS_WINDOW_HPP

#include <SDL.h>

#include <array>
#include <exception>
#include <memory>
#include <string>

#include "app.hpp"

namespace graphics {
namespace window {

void		 init();
void		 end();

class Window final {
public:
	Window()						  = delete;

	Window(const Window &)			  = delete;

	Window &operator=(const Window &) = delete;

	~Window();

	explicit Window(std::string window_name, uint32_t width = 800, uint32_t height = 800);

	void									  open();
	void									  run();
	void									  bind_app(std::unique_ptr<::app::Application> app_obj);

	[[nodiscard]] std::pair<size_t, size_t>	  size() const;

	[[nodiscard]] std::weak_ptr<SDL_Renderer> get_renderer();

	void									  quit();

private:
	struct SDLWindowDeleter {
		void operator()(SDL_Window *win) const;
	};

	struct SDLRendererDeleter {
		void operator()(SDL_Renderer *ren) const;
	};

	std::unique_ptr<SDL_Window, SDLWindowDeleter> window;
	std::shared_ptr<SDL_Renderer>				  renderer;
	std::unique_ptr<::app::Application>			  app;

	std::string									  name;
	uint32_t									  w;
	uint32_t									  h;
	bool										  should_quit;
};
}  // namespace window

class SDLException : public std::exception {
public:
	SDLException() noexcept				  = default;

	SDLException(SDLException &) noexcept = default;

	~SDLException() noexcept override	  = default;

	explicit SDLException(std::string message);

	[[nodiscard]] const char *what() const noexcept override;

private:
	std::string msg;
};

struct Color final {
public:
	explicit Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = SDL_ALPHA_OPAQUE);
	Color(const Color &)						   = default;
	Color				 &operator=(const Color &) = default;

	static const Color	  LIGHT_SQUARE;
	static const Color	  DARK_SQUARE;

	void				  rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void				  rgb(uint8_t r, uint8_t g, uint8_t b);

	void				  r(uint8_t val);
	[[nodiscard]] uint8_t r() const;

	void				  g(uint8_t val);
	[[nodiscard]] uint8_t g() const;

	void				  b(uint8_t val);
	[[nodiscard]] uint8_t b() const;

	void				  a(uint8_t val);
	[[nodiscard]] uint8_t a() const;

	explicit			  operator SDL_Color() const;

private:
	std::array<uint8_t, 4> comp;
};

}  // namespace graphics

#endif	// CHESS_WINDOW_HPP
