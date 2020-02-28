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
	// �̹������� ������ �κ� ����
	this->ref_texture_button.set_src(0, 0, image_pixel_size, image_pixel_size);

	// text texture�� ũ�� Ȯ��
	this->text_width = this->texture_text.size().x;
	this->text_height = this->texture_text.size().y;
}


Button::~Button(void) {
	this->texture_text.~Texture();
}


void Button::set_image_src_from_texture(void) noexcept {
	// ��ư�� ���¿� ���� �̹����� �ٸ��� �׸���.

	// ��ư �̹������� ������ �׸� ����
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
	// ���콺�� ��ġ�� ���� ��ư�� ���¸� �ٲٴ� �Լ�

	// ���콺�� ��ư �ۿ� ���� ��� false�� ������.
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

	// ���콺�� ��ư ���� ���ٸ� is_mouse_on = true
	if (this->is_mouse_on == false) {
		this->is_mouse_on = true;
	}
	else if (this->is_clicked == false && press == true) {
		this->is_clicked = true;
	}

	return true;
}



sdl::Rect Button::get_rect_dst(int margin) const noexcept {
	// ȭ�鿡 ����� ��ġ
	// column, line, ����, ���� ����

	// margin �ȼ� ��ŭ ������ ����. default margin = 0
	return {
		this->size_dst.x + margin,		this->size_dst.y + margin,
		this->size_dst.w - margin * 2,	this->size_dst.h - margin * 2,
	};
}


sdl::Rect Button::get_rect_src_text() const noexcept {
	// ���ڿ��� �ؽ����� ��ġ �޾ƿ��� �Լ�

	return { 0, 0, this->text_width, this->text_height };
}

void Button::set_text(const char* str_text, sdl::Renderer& render, sdl::Font const& font) noexcept {
	// ��ư �� �ؽ�Ʈ�� �����ϴ� �Լ�
	texture_text = sdl::Texture(font.make_text_texture(str_text, render.ptr()));

	// ������ �ؽ��ĸ� ����� �� ������ ũ�� ����.
	this->text_width = this->texture_text.size().x;
	this->text_height = this->texture_text.size().y;
}

void Button::draw(sdl::Renderer& render) noexcept {

	// ���콺�� ��ġ�� ���� �̹��� ����.
	this->set_image_src_from_texture();

	// ��ư �ؽ���
	render.render_copy(
		this->ref_texture_button.get_texture(),
		this->ref_texture_button.get_src(),
		this->get_rect_dst());

	// ��ư ���� ��Ÿ�� ���ڿ� ǥ��
	render.render_copy(this->texture_text, this->get_rect_src_text(), this->get_rect_dst(10));
}
