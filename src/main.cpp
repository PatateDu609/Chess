#include <iostream>
#include <vector>

#include "game/game.hpp"
#include "graphics/game.hpp"
#include "graphics/window.hpp"

int init() {
	try {
		graphics::window::init();
	} catch (const graphics::SDLException& e) {
		std::cerr << e.what();
		return -1;
	}
	return 0;
}

int main() {
	if (init()) {
		return EXIT_FAILURE;
	}

	{
		graphics::window::Window win("Chess");

		try {
			win.bind_app(std::make_unique<graphics::game::Chess>(win));
			win.open();
			win.run();
		} catch (const std::exception& e) {
			std::cerr << "caught exception: " << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}

	graphics::window::end();
	return EXIT_SUCCESS;
}
