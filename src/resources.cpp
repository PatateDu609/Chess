#include "resources.hpp"

#include <fstream>
#include <sstream>

#include "graphics/window.hpp"

namespace app::resources {

std::shared_ptr<ResourceManager<Font> > font_manager = ResourceManager<Font>::get();

Font::Font(std::filesystem::path file, uint8_t font_size)
	: filename(std::move(file)),
	  size(font_size) {
	load();
}

Font::Font(const Font& other)
	: Font(other.filename, other.size) {
}

Font& Font::operator=(const Font& other) {
	if (this == &other) return *this;

	size	 = other.size;
	filename = other.filename;
	load();

	return *this;
}

std::weak_ptr<TTF_Font> Font::get() const {
	return {font};
}

std::filesystem::path Font::get_subroot() {
	return "fonts";
}

void Font::load() {
	font.reset(TTF_OpenFont(filename.c_str(), size), TTF_CloseFont);
	if (!font) {
		throw graphics::SDLException("couldn't load font");
	}
}

Image::Image(std::filesystem::path file, size_t _w, size_t _h)
	: filename(std::move(file)),
	  image(nullptr),
	  w(_w),
	  h(_h) {
	load();
}

Image::Image(const Image& other)
	: filename(other.filename),
	  w(other.w),
	  h(other.h) {
	load();
}

Image& Image::operator=(const Image& other) {
	if (this == &other) {
		return *this;
	}

	filename = other.filename;
	w		 = other.w;
	h		 = other.h;
	load();
	return *this;
}

Image::~Image() {
	image.reset();
}

std::filesystem::path Image::get_subroot() {
	return "sprites";
}

void Image::load() {
	image.reset();

	if (filename.extension() == ".svg") {
		std::string	  content;
		std::ifstream ifs(filename);

		if (ifs) {
			std::ostringstream oss;
			oss << ifs.rdbuf();
			content = oss.str();
		}

		SDL_RWops* rw = SDL_RWFromConstMem(content.c_str(), static_cast<int>(content.length()));
		if (w != 0 && h != 0) {
			image.reset(IMG_LoadSizedSVG_RW(rw, static_cast<int>(w), static_cast<int>(h)), SDL_FreeSurface);
		} else {
			image.reset(IMG_LoadSVG_RW(rw), SDL_FreeSurface);
		}
	} else {
		image.reset(IMG_Load(filename.c_str()), SDL_FreeSurface);
	}

	if (!image) throw graphics::SDLException("couldn't load image sprite");
}

std::weak_ptr<SDL_Surface> Image::get() const {
	return {image};
}

}  // namespace app::resources
