#include <iostream>

#include "game/game.hpp"
#include "window.hpp"

int init() {
	try {
		graphics::window::init();
	} catch (const graphics::SDLException& e) {
		std::cerr << e.what();
		return -1;
	}
	return 0;
}

//int main() {
//	if (init()) {
//		return EXIT_FAILURE;
//	}
//
//	graphics::window::Window win("Chess");
//
//	win.open();
//
//	graphics::window::end();
//	return EXIT_SUCCESS;
//}


int main() {
	app::game::Board b;

	b.dump(true);

	return 0;
}
