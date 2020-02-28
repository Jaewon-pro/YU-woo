#pragma once

#define CPP_SDL2_USE_SDL_IMAGE

#include "cpp-sdl2/sdl.hpp"


class TextureManager
{
public:
	TextureManager(sdl::Renderer const&		renderer,
					std::string const&		s_path, 
					int const&				size
	);


	explicit TextureManager(SDL_Texture* p_text) noexcept;

	virtual ~TextureManager(void);


	TextureManager(TextureManager&& other) noexcept { *this = std::move(other); }

	TextureManager& operator=(TextureManager&& other) noexcept
	{
		// �̵� ���� ������
		this->texture = std::move(other.texture);
		this->src = std::move(other.src);

		return *this;
	}

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	TextureManager() = default;


	void set_src(int const& x, int const& y, int const& w, int const& h);
	void set_src(int const& col, int const& line);

	sdl::Rect get_src(void) const noexcept { return this->src; }
	sdl::Texture const& get_texture(void) const { return this->texture; }

	void draw(sdl::Renderer& r, sdl::Rect dst) const noexcept;
	
private:
	sdl::Texture		texture;

	// ���� �̹������� ������ �κ�	{���� x, ���� y, width, height}
	sdl::Rect			src; 


};


// �̹����� ������ �ʴ� �ؽ���
class Object_tex : public TextureManager
{
public:
	Object_tex(sdl::Renderer& r, std::string const& image_path, int const& size);


	~Object_tex();


	void set_image_src_from_index(int const& index) noexcept;

private:
	// png �̹������� �߶�� �� ĭ�� ũ�� ����
	const int image_pixel_size;
};


// �����ӿ� ���� src�� ���� �����̴� �ؽ���
// ���� ��â�ѵ� �׳� �������� 
class Sprite : public TextureManager
{
public:
	Sprite(sdl::Renderer& r, std::string const& image_path, int const& size, int const& frame_num);
	
	~Sprite();

	void update_animation(void) noexcept;

	
private:
	int image_pixel_size;
	int animation_frame_num;// �̹����� �ִϸ��̼� ������ ����
};
