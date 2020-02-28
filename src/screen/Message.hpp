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
	// ���ڸ� ǥ���� ����, �� ���ѵ� ���� �������� ���� ǥ��
	sdl::Rect text_boundary;

	// << �����ڷ� �Է��� ������� �����ϴ� ����
	std::wstring buffer;

	sdl::Font const& rhs_font;
	sdl::Renderer const& rhs_render;

	sdl::Color text_color = sdl::Color::White(); // ���ڻ�

	// �ؽ�Ʈ�� �ؽ��ĵ��� ����Ʈ�� ������.
	std::list<sdl::Texture> l_message_texture;

	void Message::make_texture_from_buffer(bool is_append_mode = true) noexcept;
};
