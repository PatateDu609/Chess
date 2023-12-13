#include "window.hpp"

#include <utility>


namespace graphics {
	namespace window {
		Window::Window(std::string window_name, uint32_t width, uint32_t height)
				: window(nullptr), name(std::move(window_name)), w(width), h(height) {

		}

		void Window::open() {
			window = SDL_CreateWindow(name.c_str(), 0, 0, static_cast<int>(w), static_cast<int>(h), 0);
		}

		void init() {
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
				throw SDLException("unable to initialize SDL");
			}
		}
	}

	SDLException::SDLException(std::string message) : msg(std::move(message)) {
		msg.append(": ").append(SDL_GetError());
	}

	const char *SDLException::what() const noexcept {
		return msg.c_str();
	}
}
