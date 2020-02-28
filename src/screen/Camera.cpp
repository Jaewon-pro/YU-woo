#include "Camera.hpp"

//////////////////////////////////////////////
//				class Camera


Camera::Camera(int width, int height)
	: zoom_level{ 64 }, // ǥ���� �̹����� �ȼ� ��
	column{ -width * 2 }, line{ -height * 2 },
	semi_width{ width / 2 }, semi_height{ height / 2 }
{
}

void Camera::move_camera(int const& col, int const& line) noexcept {
	// ī�޶� �̵��� ȭ�� ǥ�ô� �ݴ� �����̹Ƿ� ���ش�..
	// ���ϴ� ����� �ѹ� Ű�� ���� �� �����̴� �ȼ� ���̴�.
	auto constexpr PIXEL_MOVE = 16;

	this->column += PIXEL_MOVE * col;
	this->line += PIXEL_MOVE * line;

}


bool Camera::zoom_camera(int const& zoom) noexcept {
	// ���콺�� ��ǥ�� �����ؼ� �۵��ϴ� ī�޶� �� �Լ�

	// �ѹ� �����̴� �ȼ� ��
	int constexpr PIXEL_ZOOM = 4;

	// ȭ�鿡 ����� �̹����� �ȼ� ũ�� ������
	int constexpr MAX_ZOOM = 16;
	int constexpr MIN_ZOOM = 64;

	// zoom�� ����� ������ �̹����� Ȯ����
	this->zoom_level += PIXEL_ZOOM * zoom;

	// �ִ�� ���콺�� �Ʒ��� ����� ��, 
	// �ִ�� ���콺�� ����  �÷���  ��, ������ �۾��� �ǵ����� �Լ� ����
	if (this->zoom_level < MAX_ZOOM ||
		this->zoom_level > MIN_ZOOM) {

		this->zoom_level -= PIXEL_ZOOM * zoom;
		return false;
	}

	// â�� ũ�⿡ ���� ī�޶��� ��ġ�� ��Ÿ���� ��
	// (â ũ�� / 2 - ���� ī�޶� ��ġ) / Ȯ���ϱ����� ũ�� * �ѹ� ����� �� ���ϴ� �ȼ���

	this->column -= (this->semi_width - this->column) / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;
	this->line   -= (this->semi_height - this->line)  / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;

	return true;
}

void Camera::set_screen_size(int const& w, int const& h) noexcept {
	// ��ũ�� ũ�Ⱑ �ٲ�� �׳� ũ�⸸ �ް� ������

	// ī�޶� ���� ��ġ ȭ�� ũ�⿡ �°� �Ű���
	this->column += w / 2 - this->semi_width;
	this->line += h / 2 - this->semi_height;

	// ��ũ�� ũ�� ��ȯ ���� --> ���� ���ӽ� ���� �̿ܿ� �Ϲ����� ��쿡�� �̻� x

	this->semi_width = w / 2;
	this->semi_height = h / 2;

}



sdl::Rect Camera::get_rect_dst(int map_col, int map_line) const {
	// ī�޶� Ŭ������ ������ sdl::Rect �� ��ȯ�ؼ� ����ϴ� ��
	// �ʿ����� ��ġ�� �Ѱ��ָ� �ȼ� ������ zoom_level ���缭 ��ȯ
	// ���� ���ڿ��� �� ���� �ȼ� ������ �־ ������ ����

	return {
		this->column + (this->zoom_level * map_col),
		this->line + (this->zoom_level * map_line),
		this->zoom_level, this->zoom_level
	};
}

sdl::Rect Camera::get_rect_dst(double const& map_col, double const& map_line) const {
	// ��������Ʈ�� �� ��ġ�� �̵����̰ų� �ϸ� �Ǽ��� ��쵵 �����Ƿ�...
	// �̰͸� ������ �ٸ� draw�Լ� ������ �ʿ� ������...
	return {
		this->column + static_cast<int>(this->zoom_level * map_col),
		this->line + static_cast<int>(this->zoom_level * map_line),
		this->zoom_level, this->zoom_level
	};
}

bool Camera::find_mouse_pos(
	int const& mouse_col, int const& mouse_line,
	int& map_col, int& map_line) const

	// ���콺�� �ȼ� ��ǥ�� �޾Ƽ� ���� �� ���� ũ��� ������.
	// ���Ӿȿ����� ��ǥ�� ���۷����� ��ȯ�ϴ� �Լ�
{
	if ((mouse_col - this->column) < 0 ||
		(mouse_line - this->line) < 0)
		return false;

	map_col = (mouse_col - this->column) / zoom_level;
	map_line = (mouse_line - this->line) / zoom_level;
	return true;
}

void Camera::jump_to_loc(int const& map_col, int const& map_line) {
	// ���� ���� ��ġ�� �ް� �� ��ġ�� �̵���.

	this->column = this->semi_width - map_col * this->zoom_level;
	this->line = this->semi_height - map_line * this->zoom_level;
}
