#pragma once

#include <cpp-sdl2/ttf.hpp>

#include "ObjectManager.hpp"

// button은 카메라 이동에 영향 x
class Button
{
public:
	Button(TextureManager& te, sdl::Font const& font, sdl::Renderer& render,
		std::string text, bool const toggle,
		sdl::Rect size, int image_pixel_size
	) noexcept;

	Button(TextureManager& te, sdl::Font const& font, sdl::Renderer& render,
		std::u8string text, bool const toggle,
		sdl::Rect size, int image_pixel_size
	) noexcept;


	~Button(void);

	void set_image_src_from_texture(void) noexcept;

	bool check_mouse(int const col, int const line, bool press) noexcept;


	bool get_clicked(void)  const noexcept { return this->is_clicked; }
	bool get_on_mouse(void) const noexcept { return this->is_mouse_on; }


	void set_text(const char* str_text,
		sdl::Renderer& render, sdl::Font const& font) noexcept;

	void draw(sdl::Renderer& render) noexcept;

	Button(Button&& other) noexcept
		: ref_texture_button{ other.ref_texture_button }
	{
		*this = std::move(other);
	}

	Button& operator=(Button&& other) noexcept {
		this->is_mouse_on = std::move(other.is_mouse_on);
		this->is_clicked = std::move(other.is_clicked);
		this->is_toggle_type = std::move(other.is_toggle_type);
		
		this->size_dst = std::move(other.size_dst);
		this->image_pixel_size = std::move(other.image_pixel_size);

		this->ref_texture_button = std::move(other.ref_texture_button);
		return *this;
	}

	Button(const Button&) = delete;
	Button& operator=(const Button&) = delete;

	Button() = default;

private:
	bool is_mouse_on;
	bool is_clicked;

	bool is_toggle_type; // 캡스락처럼 한 번 누르면 계속 유지되는지 여부

	int image_pixel_size; // png 이미지에서 잘라올 한 칸의 크기 지정

	sdl::Rect size_dst;

	TextureManager& ref_texture_button;

	sdl::Texture texture_text;

	int text_width;
	int text_height;

private:
	sdl::Rect get_rect_dst(int margin = 0) const noexcept;
	sdl::Rect get_rect_src_text(void) const noexcept;

};
