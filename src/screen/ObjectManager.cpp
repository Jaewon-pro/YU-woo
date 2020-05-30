#include "ObjectManager.hpp"

#include <iostream>


TextureManager::TextureManager(sdl::Renderer const& renderer, std::string image_path, int size)

try : texture{ renderer.ptr(), image_path }, src{ 0, 0, size, size }
{
	std::clog << "Success to Load Object image: " << image_path << "\n";
}

catch (sdl::Exception const& e)
{
	std::cerr << e.what();
	std::cerr << "Fail to Load Object image" << image_path << "\n";

	sdl::show_message_box(
		SDL_MESSAGEBOX_ERROR,
		"Fail to Load Object default Image file!",
		"Please, Check the game folder. "
		"You'd like to re-download a game. ....."
	);
	exit(10);

}



TextureManager::TextureManager(SDL_Texture* p_text) noexcept
	: texture{ p_text }, src{ 0, 0, 0, 0 }
{ }

TextureManager::~TextureManager(void) {
}


void TextureManager::set_src(sdl::Rect rect) noexcept {
	this->src = std::move(rect);
}

void TextureManager::set_src(int const col, int const line) noexcept {
	this->src.x = col;
	this->src.y = line;
}


void TextureManager::draw(sdl::Renderer const& r, sdl::Rect dst) const noexcept {
	r.render_copy(this->texture, this->get_src(), dst);
}




Object_tex::Object_tex(sdl::Renderer const& r, std::string image_path, int size) noexcept
	: TextureManager{r, image_path, size}, image_pixel_size { size }
{ }

Object_tex::~Object_tex() {
	TextureManager::~TextureManager();
}


void Object_tex::set_image_src_from_index(int const index) noexcept {
	// Ÿ���� �ε����� ���ڷ� �ް� �׿� �ش��ϴ� �̹����� ��������
	// �� �̹��� ������ 10���� �� �ٷ� ����Ǿ� �ִ�.
	int constexpr NUM = 10;
	this->set_src((index % NUM) * this->image_pixel_size, (index / NUM) * this->image_pixel_size);
}




Sprite::Sprite(sdl::Renderer const& r, std::string image_path, int size, int frame_num) noexcept 
	: TextureManager{ r, image_path, size }, image_pixel_size{ size }, animation_frame_num{ frame_num }
{
}

Sprite::~Sprite(void) {
	TextureManager::~TextureManager();
}

void Sprite::update_animation(void) noexcept {
	// �ִϸ��̼ǿ� �ش��ϴ� �׸�(ȿ��)�� �Է¹��� ���簢�� �����
	// 10���� ��(col)�� �̷��� 1���� ��(line)���� �̷���.

	TextureManager::set_src(
		(TextureManager::get_src().x + this->image_pixel_size) % (this->animation_frame_num * this->image_pixel_size), 0
	);
}


