#include <iostream>
#include <vector>

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
	std::vector<app::game::Coord> coords;

	coords.emplace_back(0, 0);
	coords.emplace_back(7, 7);
	coords.emplace_back(3, 2);

	coords.emplace_back("a1");
	coords.emplace_back("h8");
	coords.emplace_back("g2");
	coords.emplace_back("b5");

	for (const auto &item : coords) {
		std::cout << item << std::endl;
	}

	return 0;
}
