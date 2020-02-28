#pragma once

#include "cpp-sdl2/sdl.hpp"

class Camera
{
public:
	Camera(int width, int height);

	void move_camera(int const& col, int const& line) noexcept;

	bool zoom_camera(int const& zoom) noexcept;

	void set_screen_size(int const& w, int const& h) noexcept;

	int get_col()	const { return this->column; }
	int get_line()	const { return this->line; }

	sdl::Rect get_rect_dst(int col, int line) const;
	sdl::Rect get_rect_dst(double const& col, double const& line) const;

	bool find_mouse_pos(int const& mouse_col, int const& mouse_line,
		int& map_col, int& map_line) const;

	void jump_to_loc(int const& col, int const& line);


private:
	// �и�up Ȯ��, ���콺 ��ũ�� ����down ��� ( �̹��� �۾��� )
	// �� ���� ���� �� �ȼ��� �����ϴ��� ������ ����
	int				zoom_level;

	// sdl::Rect { m_column, m_line, m_zoom_level (width) , m_zoom_level (height) }
	// ī�޶� ��ġ
	// ������ ȭ��ǥ�� ������ column ������ ���� �ø��� �̹����� ����� �������� �ű�.
	int				column;
	int				line;

	// ��ũ�� ũ�� / 2 ����
	int				semi_width;
	int				semi_height;

};