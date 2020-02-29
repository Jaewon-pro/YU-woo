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

	// ���� �̹������� ������ �κ�	{���� x, ���� y, width, height}
	sdl::Rect    src; 

};


// �̹����� ������ �ʴ� �ؽ���
class Object_tex : public TextureManager
{
public:
	Object_tex(sdl::Renderer const& r, std::string image_path, int size) noexcept;

	~Object_tex() noexcept;

	void set_image_src_from_index(int const index) noexcept;

private:
	// png �̹������� �߶�� �� ĭ�� ũ�� ����
	const int image_pixel_size;
};


// �����ӿ� ���� src�� ���� gif ó�� �̹����� ���ϴ� �ؽ���
class Sprite : public TextureManager
{
public:
	Sprite(sdl::Renderer const& r, std::string image_path, int size, int frame_num) noexcept;
	
	~Sprite() noexcept;

	void update_animation(void) noexcept;

	
private:
	int image_pixel_size;
	int animation_frame_num;// �̹����� �ִϸ��̼� ������ ����
};
