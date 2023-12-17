#include "resources.hpp"

#include "graphics/window.hpp"

namespace app::resources {

std::shared_ptr<ResourceManager<Font> > font_manager = ResourceManager<Font>::get();

Font::Font(std::filesystem::path file, uint8_t font_size) : filename(std::move(file)), size(font_size) {
	load();
}

Font::Font(const Font& other) : Font(other.filename, other.size) {
}

Font& Font::operator=(const Font& other) {
	if (this == &other) return *this;

	size	 = other.size;
	filename = other.filename;
	load();

	return *this;
}

Font::~Font() {
	font.reset();
}

std::weak_ptr<TTF_Font> Font::get() {
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

}  // namespace app::resources
