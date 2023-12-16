#ifndef CHESS_WINDOW_HPP
#define CHESS_WINDOW_HPP

#include <SDL.h>

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

	void									open();
	void									run();
	void									bind_app(std::unique_ptr<::app::Application> app_obj);

	[[nodiscard]] std::pair<size_t, size_t> size() const;

	[[nodiscard]] std::weak_ptr<SDL_Renderer> get_renderer();

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

}  // namespace graphics

#endif	// CHESS_WINDOW_HPP
