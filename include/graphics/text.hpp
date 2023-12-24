#ifndef CHESS_INCLUDE_GRAPHICS_TEXT_HPP
#define CHESS_INCLUDE_GRAPHICS_TEXT_HPP

#include <filesystem>

#include "resources.hpp"
#include "window.hpp"

namespace graphics {

class TextRenderer final {
public:
	explicit TextRenderer(std::shared_ptr<SDL_Renderer> renderer,
		const std::filesystem::path					   &font,
		size_t											font_size = 12);
	TextRenderer(TextRenderer &&) noexcept;

	~TextRenderer()								  = default;

	TextRenderer()								  = delete;
	TextRenderer(const TextRenderer &)			  = delete;
	TextRenderer &operator=(const TextRenderer &) = delete;

	void		  render() const;

	void		  set_content(std::string text);
	void		  set_color(const graphics::Color &col);
	void		  set_coord(size_t x, size_t y);

	void		  reload_text();

private:
	using Font		  = app::resources::Font;
	using FontManager = app::resources::ResourceManager<Font>;

	std::shared_ptr<Font>		  font;
	std::string					  content;
	std::shared_ptr<SDL_Surface>  text_surface;
	std::shared_ptr<SDL_Texture>  text_texture;
	Color						  color;
	SDL_Rect					  rect;
	std::shared_ptr<SDL_Renderer> renderer;
};

}  // namespace graphics

#endif	// CHESS_INCLUDE_GRAPHICS_TEXT_HPP
