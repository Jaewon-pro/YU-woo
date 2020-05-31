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
	// 게임 블럭 단위가 화면에서 몇 픽셀을 차지하는지 저장한 변수
	int zoom_level;

	// 게임 객체들을 그리기 시작할 카메라 위치
	int anchor_column;
	int anchor_line;

	// 스크린 크기 / 2 저장
	int semi_width;
	int semi_height;
};

