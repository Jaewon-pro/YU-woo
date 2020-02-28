#pragma once

#include <string>
#include <list>

#include "cpp-sdl2/sdl.hpp"

class Message
{
public:
	Message(sdl::Font const& font, sdl::Renderer const& render) noexcept;
	~Message();

	Message& operator<<(std::string const& str_text) noexcept;
	Message& operator<<(std::wstring const& wstr_text) noexcept;
	Message& operator<<(int num) noexcept;
	//Message& operator<<(std::u8string const& u8str_text) noexcept;


	void end_line(bool is_append_mode = true) noexcept;
	void draw(int margin = 0) const noexcept;

	void clear_list(int num = 0) noexcept;

private:
	// 글자를 표시할 영역, 이 제한된 영역 내에서만 글자 표시
	sdl::Rect text_boundary;

	// << 연산자로 입력한 문장들을 보관하는 버퍼
	std::wstring buffer;

	sdl::Font const& rhs_font;
	sdl::Renderer const& rhs_render;

	sdl::Color text_color = sdl::Color::White(); // 글자색

	// 텍스트의 텍스쳐들을 리스트로 보관함.
	std::list<sdl::Texture> l_message_texture;

	void Message::make_texture_from_buffer(bool is_append_mode = true) noexcept;
};
