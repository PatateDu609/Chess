#include <iostream>
#include "window.hpp"

int main() {
	try {
		graphics::window::init();
	} catch (const graphics::SDLException& e) {
		std::cerr << "e";
	}
}
