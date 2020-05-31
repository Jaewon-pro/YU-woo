#pragma once

#include <cpp-sdl2/sdl.hpp>

class Camera
{
public:
	Camera(int screen_width, int screen_height) noexcept;

	void move(int const dcol, int const dline) noexcept;

	bool zoom(int const zoom) noexcept;

	void set_screen_size(int const w, int const h) noexcept;

	sdl::Rect get_rect_dst(int const col, int const line) const noexcept;
	sdl::Rect get_rect_dst(double const col, double const line) const noexcept;

	bool find_mouse_pos(int const mouse_col, int const mouse_line,
		int& map_col, int& map_line) const noexcept;

	void look_at(int const map_col, int const map_line) noexcept;


private:
	// ���� �� ������ ȭ�鿡�� �� �ȼ��� �����ϴ��� ������ ����
	int zoom_level;

	// ���� ��ü���� �׸��� ������ ī�޶� ��ġ
	int anchor_column;
	int anchor_line;

	// ��ũ�� ũ�� / 2 ����
	int semi_width;
	int semi_height;
};

