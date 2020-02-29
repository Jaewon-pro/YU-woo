#pragma once

#define CPP_SDL2_USE_SDL_IMAGE

#include <cpp-sdl2/sdl.hpp>


class TextureManager
{
public:
	TextureManager(sdl::Renderer const& renderer,
		std::string s_path, int size);


	explicit TextureManager(SDL_Texture* p_text) noexcept;

	virtual ~TextureManager(void) noexcept;


	TextureManager(TextureManager&& other) noexcept { *this = std::move(other); }

	TextureManager& operator=(TextureManager&& other) noexcept
	{
		this->texture = std::move(other.texture);
		this->src = std::move(other.src);

		return *this;
	}

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	TextureManager() = default;


	void set_src(sdl::Rect rect) noexcept;
	void set_src(int const col, int const line) noexcept;

	sdl::Rect get_src(void) const noexcept { return this->src; }
	sdl::Texture const& get_texture(void) const { return this->texture; }

	void draw(sdl::Renderer const& r, sdl::Rect dst) const noexcept;
	
private:
	sdl::Texture texture;

	// 원본 이미지에서 가져올 부분	{시작 x, 시작 y, width, height}
	sdl::Rect    src; 

};


// 이미지가 변하지 않는 텍스쳐
class Object_tex : public TextureManager
{
public:
	Object_tex(sdl::Renderer const& r, std::string image_path, int size) noexcept;

	~Object_tex() noexcept;

	void set_image_src_from_index(int const index) noexcept;

private:
	// png 이미지에서 잘라올 한 칸의 크기 지정
	const int image_pixel_size;
};


// 프레임에 따라 src가 변해 gif 처럼 이미지가 변하는 텍스쳐
class Sprite : public TextureManager
{
public:
	Sprite(sdl::Renderer const& r, std::string image_path, int size, int frame_num) noexcept;
	
	~Sprite() noexcept;

	void update_animation(void) noexcept;

	
private:
	int image_pixel_size;
	int animation_frame_num;// 이미지의 애니메이션 프레임 갯수
};
