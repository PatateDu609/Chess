#ifndef CHESS_INCLUDE_RESOURCES_HPP
#define CHESS_INCLUDE_RESOURCES_HPP

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <filesystem>
#include <string>

#include "utils.hpp"

namespace app::resources {

template <typename T>
concept Resource = requires(T t) {
	{ T::get_subroot() } -> std::convertible_to<std::filesystem::path>;
};

template <Resource R>
class ResourceManager final : public utils::Singleton<ResourceManager<R> > {
public:
	ResourceManager(const ResourceManager &)			= delete;
	ResourceManager &operator=(const ResourceManager &) = delete;

	template <class... Args>
	std::shared_ptr<R> make(const std::filesystem::path &path, Args &&...args) const {
		return std::make_shared<R>(root / R::get_subroot() / path, args...);
	}

	~ResourceManager() override = default;

private:
	ResourceManager() = default;

	static std::filesystem::path root;

	friend utils::Singleton<ResourceManager<R> >;
};

template <Resource R>
std::filesystem::path ResourceManager<R>::root = "./resources";

class Font			  final {
public:
	   Font() = delete;

	   explicit Font(std::filesystem::path filename, uint8_t font_size = 12);
	   Font(const Font &other);
	   Font &operator=(const Font &other);
	   ~Font() = default;

	   [[nodiscard]] std::weak_ptr<TTF_Font> get() const;

	   static std::filesystem::path			 get_subroot();

private:
	   void						 load();

	   std::filesystem::path	 filename;
	   uint8_t					 size;
	   std::shared_ptr<TTF_Font> font;

	   friend ResourceManager<Font>;
};

class Image final {
public:
	Image() = delete;

	explicit Image(std::filesystem::path filename, size_t w, size_t h);
	Image(const Image &other);
	Image &operator=(const Image &other);
	~Image();

	[[nodiscard]] std::weak_ptr<SDL_Surface> get() const;

	static std::filesystem::path			 get_subroot();

private:
	void						 load();

	std::filesystem::path		 filename;
	std::shared_ptr<SDL_Surface> image;
	size_t						 w;
	size_t						 h;

	friend ResourceManager<Image>;
};

extern std::shared_ptr<ResourceManager<Font> >	font_manager;
extern std::shared_ptr<ResourceManager<Image> > image_manager;

}  // namespace app::resources

#endif	// CHESS_RESOURCES_HPP
