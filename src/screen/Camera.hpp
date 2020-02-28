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
	// 밀면up 확대, 마우스 스크롤 당기면down 축소 ( 이미지 작아짐 )
	// 한 게임 블럭이 몇 픽셀을 차지하는지 저장한 변수
	int				zoom_level;

	// sdl::Rect { m_column, m_line, m_zoom_level (width) , m_zoom_level (height) }
	// 카메라 위치
	// 오른쪽 화살표를 누른건 column 변수의 값을 늘리고 이미지의 출력을 왼쪽으로 옮김.
	int				column;
	int				line;

	// 스크린 크기 / 2 저장
	int				semi_width;
	int				semi_height;

};