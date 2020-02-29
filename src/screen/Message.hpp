#pragma once

#include <string>
#include <list>

#include <cpp-sdl2/sdl.hpp>

class Message
{
public:
	Message(sdl::Font const& font, sdl::Renderer const& render) noexcept;
	~Message();

	template <typename Type_text>
	Message& operator<<(Type_text text) noexcept {
		this->buffer += text;
		return *this;
	}


	void end_line(void) noexcept;
	void draw(int margin = 0) const noexcept;

	void clear_list(int num = 0) noexcept;

private:
	// 글자를 표시할 영역, 이 제한된 영역 내에서만 글자 표시
	sdl::Rect text_boundary;

	// << 연산자로 입력한 문장들을 보관하는 버퍼
	std::wstring buffer;

	sdl::Font const& ref_font;
	sdl::Renderer const& ref_render;

	sdl::Color text_color = sdl::Color::White(); // 글자색

	// 텍스트의 텍스쳐들을 리스트로 보관함.
	std::list<sdl::Texture> l_message_texture;

	void Message::make_texture_from_buffer(void) noexcept;
};

