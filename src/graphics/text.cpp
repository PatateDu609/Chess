#include "graphics/text.hpp"

#include <utility>

namespace graphics {

TextRenderer::TextRenderer(std::shared_ptr<SDL_Renderer> r,
	const std::filesystem::path							&font_path,
	size_t												 font_size)
	: renderer(std::move(r)),
	  font(FontManager::get()->make(font_path, font_size)),
	  rect(0, 0, 0, 0),
	  color(0, 0, 0),
	  content() {
}

TextRenderer::TextRenderer(TextRenderer &&other) noexcept
	: renderer(std::move(other.renderer)),
	  font(std::move(other.font)),
	  rect(other.rect),
	  color(other.color),
	  content(std::move(other.content)) {
}

void TextRenderer::render() const {
	if (auto f = font->get().lock()) {
		SDL_RenderCopy(renderer.get(), text_texture.get(), nullptr, &rect);
	} else {
		throw std::runtime_error("no font available: font did expire");
	}
}

void TextRenderer::set_content(std::string text) {
	content = std::move(text);
}

void TextRenderer::set_color(const graphics::Color &c) {
	color = c;
}

void TextRenderer::set_coord(size_t x, size_t y) {
	rect.x = static_cast<int>(x);
	rect.y = static_cast<int>(y);
}

void TextRenderer::reload_text() {
	if (auto f = font->get().lock()) {

		auto sdlColor = static_cast<SDL_Color>(color);

		text_surface.reset(TTF_RenderText_Blended(f.get(), content.c_str(), sdlColor), SDL_FreeSurface);
		if (!text_surface) {
			throw graphics::SDLException("couldn't render text");
		}

		rect.w = text_surface->w;
		rect.h = text_surface->h;

		text_texture.reset(SDL_CreateTextureFromSurface(renderer.get(), text_surface.get()),
			SDL_DestroyTexture);
		if (!text_texture) {
			throw graphics::SDLException("couldn't create texture from surface");
		}
	}
}

}  // namespace graphics
