#include "window.hpp"

#include <utility>

namespace graphics {
namespace window {
Window::Window(std::string window_name, uint32_t width, uint32_t height)
	: window(nullptr, SDLWindowDeleter()),
	  name(std::move(window_name)),
	  w(width),
	  h(height) {}

void Window::open() {
	window.reset(SDL_CreateWindow(
		name.c_str(), 0, 0, static_cast<int>(w), static_cast<int>(h), 0));
	if (window == nullptr) {
		throw SDLException("couldn't create window");
	}
}

void Window::run() {
	bool quit = false;

	while (!quit) {
		SDL_Event e;

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else {
				app->handle_events(e);
			}
		}

		app->draw();
	}
}

void Window::bind_app(std::unique_ptr<app::Application> &app_obj) {
	app = std::move(app_obj);
}

void Window::SDLWindowDeleter::operator()(SDL_Window *win) const {
	SDL_DestroyWindow(win);
}

void init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		throw SDLException("unable to initialize SDL");
	}
}

void end() { SDL_Quit(); }
}  // namespace window

SDLException::SDLException(std::string message) : msg(std::move(message)) {
	msg.append(": ").append(SDL_GetError());
}

const char *SDLException::what() const noexcept { return msg.c_str(); }
}  // namespace graphics
