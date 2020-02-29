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
	// ���ڸ� ǥ���� ����, �� ���ѵ� ���� �������� ���� ǥ��
	sdl::Rect text_boundary;

	// << �����ڷ� �Է��� ������� �����ϴ� ����
	std::wstring buffer;

	sdl::Font const& ref_font;
	sdl::Renderer const& ref_render;

	sdl::Color text_color = sdl::Color::White(); // ���ڻ�

	// �ؽ�Ʈ�� �ؽ��ĵ��� ����Ʈ�� ������.
	std::list<sdl::Texture> l_message_texture;

	void Message::make_texture_from_buffer(void) noexcept;
};

