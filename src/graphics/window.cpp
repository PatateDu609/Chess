#include "graphics/window.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <utility>

namespace graphics {
namespace window {

Window::Window(std::string window_name, uint32_t width, uint32_t height)
	: window(nullptr, SDLWindowDeleter()),
	  renderer(),
	  name(std::move(window_name)),
	  w(width),
	  h(height),
	  should_quit(false) {
}

Window::~Window() {
	renderer.reset();
	window.reset();
}

void Window::open() {
	window.reset(SDL_CreateWindow(name.c_str(), 0, 0, static_cast<int>(w), static_cast<int>(h), 0));
	if (window == nullptr) {
		throw SDLException("couldn't create window");
	}

	constexpr Uint32 SDL_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_renderer_flags), SDLRendererDeleter());
}

std::weak_ptr<SDL_Renderer> Window::get_renderer() {
	return {renderer};
}

void Window::quit() {
	should_quit = true;
}

void Window::run() {
	while (!should_quit) {
		SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer.get());

		SDL_Event e;

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit();
			} else {
				app->handle_events(e);
			}
		}

		app->update();
		app->draw();
		SDL_RenderPresent(renderer.get());
	}
}

void Window::bind_app(std::unique_ptr<app::Application> app_obj) {
	app = std::move(app_obj);
}

std::pair<size_t, size_t> Window::size() const {
	return std::make_pair(w, h);
}

void Window::SDLWindowDeleter::operator()(SDL_Window *win) const {
	SDL_DestroyWindow(win);
}

void Window::SDLRendererDeleter::operator()(SDL_Renderer *ren) const {
	SDL_DestroyRenderer(ren);
}

void init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		throw SDLException("unable to initialize SDL");
	}

	if (TTF_Init() != 0) {
		throw SDLException("unable to initialize TTF lib");
	}

	int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP | IMG_INIT_JXL | IMG_INIT_AVIF;
	if (IMG_Init(flags) == 0) {
		throw SDLException("unable to initialize IMG lib");
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
}

void end() {
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

}  // namespace window

SDLException::SDLException(std::string message)
	: msg(std::move(message)) {
	msg.append(": ").append(SDL_GetError());
}

const char *SDLException::what() const noexcept {
	return msg.c_str();
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	: comp{r, g, b, a} {
}

void Color::rgba(uint8_t rVal, uint8_t gVal, uint8_t bVal, uint8_t aVal) {
	comp[0] = rVal;
	comp[1] = bVal;
	comp[2] = gVal;
	comp[3] = aVal;
}

void Color::rgb(uint8_t rVal, uint8_t gVal, uint8_t bVal) {
	rgba(rVal, gVal, bVal, a());
}

void Color::r(uint8_t val) {
	comp[0] = val;
}

uint8_t Color::r() const {
	return comp[0];
}

void Color::g(uint8_t val) {
	comp[1] = val;
}

uint8_t Color::g() const {
	return comp[1];
}

void Color::b(uint8_t val) {
	comp[2] = val;
}

uint8_t Color::b() const {
	return comp[2];
}

void Color::a(uint8_t val) {
	comp[3] = val;
}

uint8_t Color::a() const {
	return comp[3];
}

Color::operator SDL_Color() const {
	SDL_Color col;

	col.a = a();
	col.r = r();
	col.g = g();
	col.b = b();
	return col;
}

const Color Color::LIGHT_SQUARE(237, 214, 175);
const Color Color::DARK_SQUARE(184, 135, 97);

}  // namespace graphics
