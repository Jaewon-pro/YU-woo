#include "Button.hpp"

#include <iostream>

enum class BUTTON : int {
	OFF, ON, CLICK
};


Button::Button(TextureManager& te, sdl::Font const& font, sdl::Renderer& render,
	std::string text,
	bool const toggle,
	sdl::Rect size,
	int image_pixel_size) noexcept
	: Button(te, font, render, font.string2wstr(text).c_str(),
		toggle, size, image_pixel_size)
{
}

Button::Button(TextureManager& te, sdl::Font const& font, sdl::Renderer& render,
	std::wstring text, bool const toggle, sdl::Rect size, int image_pixel_size) noexcept
	: is_mouse_on{ false }, is_clicked{ false }
	, is_toggle_type{ toggle }
	, image_pixel_size{ image_pixel_size }
	, size_dst{ size }
	, ref_texture_button{ te }
	, texture_text{ font.make_text_texture( text.c_str(), render.ptr() ) }
{
	// 이미지에서 가져올 부분 설정
	this->ref_texture_button.set_src(0, 0, image_pixel_size, image_pixel_size);

	// text texture의 크기 확인
	this->text_width = this->texture_text.size().x;
	this->text_height = this->texture_text.size().y;
}


Button::~Button(void) {
	this->texture_text.~Texture();
}


void Button::set_image_src_from_texture(void) noexcept {
	// 버튼의 상태에 따라 이미지를 다르게 그린다.

	// 버튼 이미지에서 가져올 테마 선택
	int constexpr BUTTON_THEMA = 2;

	if (this->is_mouse_on == true) {
		this->ref_texture_button.set_src(
			BUTTON_THEMA * this->image_pixel_size, this->image_pixel_size * static_cast<int>(BUTTON::ON));
	} else {
		this->ref_texture_button.set_src(
			BUTTON_THEMA * this->image_pixel_size, this->image_pixel_size * static_cast<int>(BUTTON::OFF));
		return;
	}

	if (this->is_clicked == true) {
		this->ref_texture_button.set_src(
			BUTTON_THEMA * this->image_pixel_size, this->image_pixel_size * static_cast<int>(BUTTON::CLICK));
	}
	return;
}



bool Button::check_mouse(int const& col, int const& line, bool press) noexcept {
	// 마우스의 위치에 따라 버튼의 상태를 바꾸는 함수

	// 마우스가 버튼 밖에 있을 경우 false를 리턴함.
	if (this->size_dst.x > col || this->size_dst.x + this->size_dst.w < col) {
		// 
		//if (this->mb_is_mouse_on == false) return true;

		this->is_mouse_on = false;
		this->is_clicked = false;
		return false;
	}
	if (this->size_dst.y > line || this->size_dst.y + this->size_dst.h < line) {
		//if (this->mb_is_mouse_on == false) return true;

		this->is_mouse_on = false;
		this->is_clicked = false;
		return false;
	}

	// 마우스가 버튼 위에 없다면 is_mouse_on = true
	if (this->is_mouse_on == false) {
		this->is_mouse_on = true;
	}
	else if (this->is_clicked == false && press == true) {
		this->is_clicked = true;
	}

	return true;
}



sdl::Rect Button::get_rect_dst(int margin) const noexcept {
	// 화면에 출력할 위치
	// column, line, 가로, 세로 길이

	// margin 픽셀 만큼 여백을 만듦. default margin = 0
	return {
		this->size_dst.x + margin,		this->size_dst.y + margin,
		this->size_dst.w - margin * 2,	this->size_dst.h - margin * 2,
	};
}


sdl::Rect Button::get_rect_src_text() const noexcept {
	// 문자열의 텍스쳐의 위치 받아오는 함수

	return { 0, 0, this->text_width, this->text_height };
}

void Button::set_text(const char* str_text, sdl::Renderer& render, sdl::Font const& font) noexcept {
	// 버튼 위 텍스트를 변경하는 함수
	texture_text = sdl::Texture(font.make_text_texture(str_text, render.ptr()));

	// 문자의 텍스쳐를 만들고 그 원본의 크기 저장.
	this->text_width = this->texture_text.size().x;
	this->text_height = this->texture_text.size().y;
}

void Button::draw(sdl::Renderer& render) noexcept {

	// 마우스의 위치에 따라 이미지 정함.
	this->set_image_src_from_texture();

	// 버튼 텍스쳐
	render.render_copy(
		this->ref_texture_button.get_texture(),
		this->ref_texture_button.get_src(),
		this->get_rect_dst());

	// 버튼 위에 나타낼 문자열 표시
	render.render_copy(this->texture_text, this->get_rect_src_text(), this->get_rect_dst(10));
}
