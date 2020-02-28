#include "Camera.hpp"

//////////////////////////////////////////////
//				class Camera


Camera::Camera(int width, int height)
	: zoom_level{ 64 }, // 표시할 이미지의 픽셀 수
	column{ -width * 2 }, line{ -height * 2 },
	semi_width{ width / 2 }, semi_height{ height / 2 }
{
}

void Camera::move_camera(int const& col, int const& line) noexcept {
	// 카메라 이동과 화면 표시는 반대 방향이므로 빼준다..
	// 곱하는 상수는 한번 키를 누를 때 움직이는 픽셀 수이다.
	auto constexpr PIXEL_MOVE = 16;

	this->column += PIXEL_MOVE * col;
	this->line += PIXEL_MOVE * line;

}


bool Camera::zoom_camera(int const& zoom) noexcept {
	// 마우스의 좌표와 관련해서 작동하는 카메라 줌 함수

	// 한번 움직이는 픽셀 수
	int constexpr PIXEL_ZOOM = 4;

	// 화면에 출력할 이미지의 픽셀 크기 상하한
	int constexpr MAX_ZOOM = 16;
	int constexpr MIN_ZOOM = 64;

	// zoom이 양수면 각각의 이미지는 확대함
	this->zoom_level += PIXEL_ZOOM * zoom;

	// 최대로 마우스를 아래로 당겼을 때, 
	// 최대로 마우스를 위로  올렸을  때, 이전의 작업을 되돌리고 함수 종료
	if (this->zoom_level < MAX_ZOOM ||
		this->zoom_level > MIN_ZOOM) {

		this->zoom_level -= PIXEL_ZOOM * zoom;
		return false;
	}

	// 창의 크기에 맞춰 카메라의 위치를 나타내는 식
	// (창 크기 / 2 - 현재 카메라 위치) / 확대하기전의 크기 * 한번 축소할 떄 변하는 픽셀수

	this->column -= (this->semi_width - this->column) / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;
	this->line   -= (this->semi_height - this->line)  / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;

	return true;
}

void Camera::set_screen_size(int const& w, int const& h) noexcept {
	// 스크린 크기가 바뀔시 그냥 크기만 받고 설정함

	// 카메라 시작 위치 화면 크기에 맞게 옮겨줌
	this->column += w / 2 - this->semi_width;
	this->line += h / 2 - this->semi_height;

	// 스크린 크기 변환 오류 --> 원격 접속시 오류 이외외 일반적인 경우에는 이상 x

	this->semi_width = w / 2;
	this->semi_height = h / 2;

}



sdl::Rect Camera::get_rect_dst(int map_col, int map_line) const {
	// 카메라 클래스의 목적은 sdl::Rect 로 변환해서 사용하는 것
	// 맵에서의 위치만 넘겨주면 픽셀 사이즈 zoom_level 맞춰서 반환
	// 따라서 인자에는 한 블럭의 픽셀 사이즈 넣어서 보내지 마셈

	return {
		this->column + (this->zoom_level * map_col),
		this->line + (this->zoom_level * map_line),
		this->zoom_level, this->zoom_level
	};
}

sdl::Rect Camera::get_rect_dst(double const& map_col, double const& map_line) const {
	// 스프라이트의 맵 위치가 이동중이거나 하면 실수인 경우도 있으므로...
	// 이것만 있으면 다른 draw함수 수정할 필요 없겠지...
	return {
		this->column + static_cast<int>(this->zoom_level * map_col),
		this->line + static_cast<int>(this->zoom_level * map_line),
		this->zoom_level, this->zoom_level
	};
}

bool Camera::find_mouse_pos(
	int const& mouse_col, int const& mouse_line,
	int& map_col, int& map_line) const

	// 마우스의 픽셀 좌표를 받아서 현재 한 블럭의 크기로 나눈다.
	// 게임안에서의 좌표를 레퍼런스로 반환하는 함수
{
	if ((mouse_col - this->column) < 0 ||
		(mouse_line - this->line) < 0)
		return false;

	map_col = (mouse_col - this->column) / zoom_level;
	map_line = (mouse_line - this->line) / zoom_level;
	return true;
}

void Camera::jump_to_loc(int const& map_col, int const& map_line) {
	// 게임 맵의 위치를 받고 그 위치로 이동함.

	this->column = this->semi_width - map_col * this->zoom_level;
	this->line = this->semi_height - map_line * this->zoom_level;
}
