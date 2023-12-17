#ifndef CHESS_IMAGE_HPP
#define CHESS_IMAGE_HPP

#include <SDL.h>

#include <filesystem>

#include "resources.hpp"

namespace graphics {

class ImageRenderer final {
public:
	explicit ImageRenderer(const std::filesystem::path &filename, size_t w, size_t h);
	ImageRenderer(graphics::ImageRenderer &&other) noexcept;
	~ImageRenderer()								= default;

	ImageRenderer()									= delete;
	ImageRenderer(const ImageRenderer &)			= default;
	ImageRenderer &operator=(const ImageRenderer &) = default;

	void		   render(const std::shared_ptr<SDL_Renderer> &renderer) const;

	void		   set_size(size_t w, size_t h);
	void		   set_coord(size_t x, size_t y);

private:
	using Image		   = app::resources::Image;
	using ImageManager = app::resources::ResourceManager<Image>;

	SDL_Rect			   rect;
	std::shared_ptr<Image> image;
};

}  // namespace graphics

#endif	// CHESS_IMAGE_HPP
