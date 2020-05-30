#include "Camera.hpp"


Camera::Camera(int width, int height) noexcept
	: zoom_level{ 64 }
	, anchor_column{ -width * 2 }
	, anchor_line{ -height * 2 }
	, semi_width{ width / 2 }
	, semi_height{ height / 2 }
{ }

void Camera::move_camera(int const col, int const line) noexcept {
	// 곱하는 상수는 한번 키를 누를 때 움직이는 픽셀 수이다.
	int constexpr PIXEL_MOVE = 16;

	this->anchor_column += PIXEL_MOVE * col;
	this->anchor_line += PIXEL_MOVE * line;
}


bool Camera::zoom_camera(int const zoom) noexcept {
	// 마우스의 좌표와 관련해서 작동하는 카메라 줌 함수

	// 한번 움직이는 픽셀 수
	int constexpr PIXEL_ZOOM = 4;

	// 화면에 출력할 이미지의 픽셀 크기 상하한
	int constexpr MAX_ZOOM = 16;
	int constexpr MIN_ZOOM = 64;

	// zoom이 양수면 각각의 이미지를 확대함
	this->zoom_level += PIXEL_ZOOM * zoom;

	// 카메라 확대, 축소의 경계를 벗어난 경우 원상복구
	if (this->zoom_level < MAX_ZOOM || this->zoom_level > MIN_ZOOM) {

		this->zoom_level -= PIXEL_ZOOM * zoom;
		return false;
	}

	// 창의 크기에 맞춰 카메라의 위치를 나타내는 식
	// (창 크기 / 2 - 현재 카메라 위치) / 확대하기전의 크기 * 한번 축소할 떄 변하는 픽셀수

	this->anchor_column -= (this->semi_width - this->anchor_column) / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;
	this->anchor_line   -= (this->semi_height - this->anchor_line)  / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;

	return true;
}

void Camera::set_screen_size(int const w, int const h) noexcept {
	// 스크린 크기가 바뀔시 카메라 중심점을 동일하게 함

	// 카메라 앵커 위치를 화면 크기에 맞게 옮겨줌
	this->anchor_column += w / 2 - this->semi_width;
	this->anchor_line += h / 2 - this->semi_height;

	this->semi_width = w / 2;
	this->semi_height = h / 2;

}



sdl::Rect Camera::get_rect_dst(int const map_col, int const map_line) const noexcept {
	// 맵에서의 위치만 넘겨주면 픽셀 사이즈 zoom_level 맞춰서 반환

	return {
		this->anchor_column + (this->zoom_level * map_col),
		this->anchor_line + (this->zoom_level * map_line),
		this->zoom_level, this->zoom_level
	};
}

sdl::Rect Camera::get_rect_dst(double const map_col, double const map_line) const noexcept {

	return {
		this->anchor_column + static_cast<int>(this->zoom_level * map_col),
		this->anchor_line   + static_cast<int>(this->zoom_level * map_line),
		this->zoom_level, this->zoom_level
	};
}

bool Camera::find_mouse_pos(int const mouse_col, int const mouse_line,
	int& map_col, int& map_line) const noexcept {

	// 마우스의 픽셀 좌표를 받아서 현재 한 블럭의 크기로 나눈다.
	// 게임안에서의 좌표를 레퍼런스로 반환하는 함수

	if ((mouse_col - this->anchor_column) < 0 ||
		(mouse_line - this->anchor_line) < 0)
		return false;

	map_col = (mouse_col - this->anchor_column) / zoom_level;
	map_line = (mouse_line - this->anchor_line) / zoom_level;
	return true;
}

void Camera::jump_to_location(int const map_col, int const map_line) noexcept {
	// 게임 맵의 위치를 받고 그 위치로 이동함.

	this->anchor_column = this->semi_width - map_col * this->zoom_level;
	this->anchor_line = this->semi_height - map_line * this->zoom_level;
}
