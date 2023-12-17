#ifndef CHESS_APP_HPP
#define CHESS_APP_HPP

#include <SDL.h>

namespace graphics::window {
class Window;
}

namespace app {
class Application {
public:
	Application(const Application&)			   = delete;
	Application& operator=(const Application&) = delete;

	Application()							   = default;
	virtual ~Application()					   = default;

protected:
	virtual void update()						   = 0;
	virtual void draw() const					   = 0;
	virtual void handle_events(const SDL_Event& e) = 0;

	friend class ::graphics::window::Window;
};
}  // namespace app

#endif	// CHESS_APP_HPP
