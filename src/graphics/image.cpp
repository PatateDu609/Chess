#include "graphics/image.hpp"

#include "graphics/window.hpp"

namespace graphics {

ImageRenderer::ImageRenderer(const std::filesystem::path &filename, size_t w, size_t h)
	: image(ImageManager::get()->make(filename, w, h)),
	  rect(0, 0, static_cast<int>(w), static_cast<int>(h)) {
}

ImageRenderer::ImageRenderer(graphics::ImageRenderer &&other) noexcept
	: rect(other.rect),
	  image(std::move(other.image)) {
}

void ImageRenderer::render(const std::shared_ptr<SDL_Renderer> &renderer) const {
	if (auto surface = image->get().lock()) {
		std::shared_ptr<SDL_Texture> texture(SDL_CreateTextureFromSurface(renderer.get(), surface.get()),
			SDL_DestroyTexture);
		if (!texture) {
			throw SDLException("couldn't create texture");
		}

		SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &rect);
	} else {
		throw std::runtime_error("couldn't lock image");
	}
}

void ImageRenderer::set_rect(const SDL_Rect &sdlRect) {
	rect = sdlRect;
}

void ImageRenderer::set_rect(size_t x, size_t y, size_t w, size_t h) {
	set_size(w, h);
	set_coord(x, y);
}

void ImageRenderer::set_size(size_t w, size_t h) {
	rect.w = static_cast<int>(w);
	rect.h = static_cast<int>(h);
}

void ImageRenderer::set_coord(size_t x, size_t y) {
	rect.x = static_cast<int>(x);
	rect.y = static_cast<int>(y);
}

}  // namespace graphics
