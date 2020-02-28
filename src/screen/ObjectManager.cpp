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
		// unknown.png도 없는 경우
		std::cerr << "Fail to load Object image: unknown.png\n";
		sdl::show_message_box(
			SDL_MESSAGEBOX_ERROR,
			"Fail to Load Object default Image file!", // 제목
			"Please, Check the game folder. "
			"You'd like to re-download a game. ....."
		);
		exit(10);
	}
	else {
		// 원하는 이미지는 없고 unknown.png만 불러올 수 있는 경우

		//*this = ObjectManager(p_t);
		//std::clog << this->get_t().ptr();
		std::cerr << "Success to Load Object image unknown.png \n";
		sdl::show_message_box(
			SDL_MESSAGEBOX_ERROR,
			"Fail to Load Object Image file!", // 제목
			"Please, Check the game folder. "
			"You'd like to re-download a game. ....."
		);
		// 원래는 이동대입 연산을 해서 자신을 초기화 해줘야 하는데 안 배워서 모르겠음...
		// 대입 연산자의 문제인가... 모르겠다.
		// 그래서 일단 종료하도록 해놓음
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
	// 타일의 인덱스를 인자로 받고 그에 해당하는 이미지로 설정해줌
	// tils의 이미지는 타일 10개씩 한줄로 저장되어 있다.
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
	// 애니메이션에 해당하는 그림(효과)는 입력받은 정사각형 사이즈가
	// 10개의 열(col)로 이뤄진 1개의 행(line)으로 이뤄짐.

	// 따라서 시간의 흐름에 따라 1칸씩 이동해 그리면 됨
	// 이미지 전환이 너무 빠르다...
	TextureManager::set_src(
		(TextureManager::get_src().x + this->image_pixel_size) % (this->animation_frame_num * this->image_pixel_size), 0
	);
}


