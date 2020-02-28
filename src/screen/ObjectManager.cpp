#include "ObjectManager.hpp"

#include <iostream>


//////////////////////////////////////////////////////
//				class TextureManager defintion



TextureManager::TextureManager(sdl::Renderer const& renderer, std::string const& s_path, int const& size)

try : src{ 0, 0, size, size }, texture{ renderer.ptr(), s_path }
{
	std::clog << "Success to Load Object image: " << s_path << "\n";
}

catch (sdl::Exception & e)
{
	std::cerr << e.what();
	std::cerr << "\nSo, Use unknown.png\n";

	//SDL_Texture* p_t = this->load_texture("unknown.png", renderer.ptr());
	SDL_Texture* p_t = nullptr;

	if (p_t == nullptr) {
		// unknown.png�� ���� ���
		std::cerr << "Fail to load Object image: unknown.png\n";
		sdl::show_message_box(
			SDL_MESSAGEBOX_ERROR,
			"Fail to Load Object default Image file!", // ����
			"Please, Check the game folder. "
			"You'd like to re-download a game. ....."
		);
		exit(10);
	}
	else {
		// ���ϴ� �̹����� ���� unknown.png�� �ҷ��� �� �ִ� ���

		//*this = ObjectManager(p_t);
		//std::clog << this->get_t().ptr();
		std::cerr << "Success to Load Object image unknown.png \n";
		sdl::show_message_box(
			SDL_MESSAGEBOX_ERROR,
			"Fail to Load Object Image file!", // ����
			"Please, Check the game folder. "
			"You'd like to re-download a game. ....."
		);
		// ������ �̵����� ������ �ؼ� �ڽ��� �ʱ�ȭ ����� �ϴµ� �� ����� �𸣰���...
		// ���� �������� �����ΰ�... �𸣰ڴ�.
		// �׷��� �ϴ� �����ϵ��� �س���
		exit(11);
	}
	//SDL_DestroyTexture(p_t);

}


TextureManager::TextureManager(SDL_Texture* p_text) noexcept
	: texture{ p_text }, src{ 0, 0, 200, 200 }
{
}

TextureManager::~TextureManager(void) {
	this->texture.~Texture();
}



/////////////////////////////////////////////////////////////////////
// public function



void TextureManager::set_src(int const& x, int const& y, int const& w, int const& h) {
	this->src.x = x;	this->src.y = y;	this->src.w = w;	this->src.h = h;
}

void TextureManager::set_src(int const& col, int const& line) { this->src.x = col;	this->src.y = line; }


void TextureManager::draw(sdl::Renderer& r, sdl::Rect dst) const noexcept {
	r.render_copy(this->texture, this->get_src(), dst);
}

/////////////////////////////////////////////////////////////////////
// private function





/////////////////////////////////////////////////////////////////////
//					class Object_tex def

Object_tex::Object_tex(sdl::Renderer& r, std::string const& image_path, int const& size)
	: TextureManager{r, image_path, size}, image_pixel_size { size }
{
}


Object_tex::~Object_tex() {
	TextureManager::~TextureManager();
}


void Object_tex::set_image_src_from_index(int const& index) noexcept {
	// Ÿ���� �ε����� ���ڷ� �ް� �׿� �ش��ϴ� �̹����� ��������
	// tils�� �̹����� Ÿ�� 10���� ���ٷ� ����Ǿ� �ִ�.
	auto constexpr NUM = 10;
	this->set_src((index % NUM) * this->image_pixel_size, (index / NUM) * this->image_pixel_size);
}

/////////////////////////////////////////////////////////////////////
//					class Sprite def

Sprite::Sprite(sdl::Renderer& r, std::string const& image_path, int const& size, int const& frame_num)
	: TextureManager{ r, image_path, size }, image_pixel_size{ size }, animation_frame_num{ frame_num }
{
}

Sprite::~Sprite(void) {
	TextureManager::~TextureManager();
}

void Sprite::update_animation(void) noexcept {
	// �ִϸ��̼ǿ� �ش��ϴ� �׸�(ȿ��)�� �Է¹��� ���簢�� �����
	// 10���� ��(col)�� �̷��� 1���� ��(line)���� �̷���.

	// ���� �ð��� �帧�� ���� 1ĭ�� �̵��� �׸��� ��
	// �̹��� ��ȯ�� �ʹ� ������...
	TextureManager::set_src(
		(TextureManager::get_src().x + this->image_pixel_size) % (this->animation_frame_num * this->image_pixel_size), 0
	);
}


