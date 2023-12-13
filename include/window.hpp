#ifndef CHESS_WINDOW_HPP
#define CHESS_WINDOW_HPP

#include <SDL.h>

#include <string>
#include <exception>

namespace graphics {
	namespace window {
		void init();

		class Window final {
		public:
			Window() = delete;

			Window(const Window &) = delete;

			Window &operator=(const Window &) = delete;

			void open();

			explicit Window(std::string window_name, uint32_t width = 800, uint32_t height = 800);

		private:
			SDL_Window *window;

			std::string name;
			uint32_t w;
			uint32_t h;
		};
	}

	class SDLException : public std::exception {
	public:
		SDLException() noexcept = default;

		SDLException(SDLException &) noexcept = default;

		~SDLException() noexcept override = default;

		explicit SDLException(std::string message);

		[[nodiscard]] const char *what() const noexcept override;

	private:
		std::string msg;
	};

}

#endif //CHESS_WINDOW_HPP
