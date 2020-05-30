#include "Camera.hpp"


Camera::Camera(int width, int height) noexcept
	: zoom_level{ 64 }
	, anchor_column{ -width * 2 }
	, anchor_line{ -height * 2 }
	, semi_width{ width / 2 }
	, semi_height{ height / 2 }
{ }

void Camera::move_camera(int const col, int const line) noexcept {
	// ���ϴ� ����� �ѹ� Ű�� ���� �� �����̴� �ȼ� ���̴�.
	int constexpr PIXEL_MOVE = 16;

	this->anchor_column += PIXEL_MOVE * col;
	this->anchor_line += PIXEL_MOVE * line;
}


bool Camera::zoom_camera(int const zoom) noexcept {
	// ���콺�� ��ǥ�� �����ؼ� �۵��ϴ� ī�޶� �� �Լ�

	// �ѹ� �����̴� �ȼ� ��
	int constexpr PIXEL_ZOOM = 4;

	// ȭ�鿡 ����� �̹����� �ȼ� ũ�� ������
	int constexpr MAX_ZOOM = 16;
	int constexpr MIN_ZOOM = 64;

	// zoom�� ����� ������ �̹����� Ȯ����
	this->zoom_level += PIXEL_ZOOM * zoom;

	// ī�޶� Ȯ��, ����� ��踦 ��� ��� ���󺹱�
	if (this->zoom_level < MAX_ZOOM || this->zoom_level > MIN_ZOOM) {

		this->zoom_level -= PIXEL_ZOOM * zoom;
		return false;
	}

	// â�� ũ�⿡ ���� ī�޶��� ��ġ�� ��Ÿ���� ��
	// (â ũ�� / 2 - ���� ī�޶� ��ġ) / Ȯ���ϱ����� ũ�� * �ѹ� ����� �� ���ϴ� �ȼ���

	this->anchor_column -= (this->semi_width - this->anchor_column) / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;
	this->anchor_line   -= (this->semi_height - this->anchor_line)  / (this->zoom_level - PIXEL_ZOOM * zoom) * PIXEL_ZOOM * zoom;

	return true;
}

void Camera::set_screen_size(int const w, int const h) noexcept {
	// ��ũ�� ũ�Ⱑ �ٲ�� ī�޶� �߽����� �����ϰ� ��

	// ī�޶� ��Ŀ ��ġ�� ȭ�� ũ�⿡ �°� �Ű���
	this->anchor_column += w / 2 - this->semi_width;
	this->anchor_line += h / 2 - this->semi_height;

	this->semi_width = w / 2;
	this->semi_height = h / 2;

}



sdl::Rect Camera::get_rect_dst(int const map_col, int const map_line) const noexcept {
	// �ʿ����� ��ġ�� �Ѱ��ָ� �ȼ� ������ zoom_level ���缭 ��ȯ

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

	// ���콺�� �ȼ� ��ǥ�� �޾Ƽ� ���� �� ���� ũ��� ������.
	// ���Ӿȿ����� ��ǥ�� ���۷����� ��ȯ�ϴ� �Լ�

	if ((mouse_col - this->anchor_column) < 0 ||
		(mouse_line - this->anchor_line) < 0)
		return false;

	map_col = (mouse_col - this->anchor_column) / zoom_level;
	map_line = (mouse_line - this->anchor_line) / zoom_level;
	return true;
}

void Camera::jump_to_location(int const map_col, int const map_line) noexcept {
	// ���� ���� ��ġ�� �ް� �� ��ġ�� �̵���.

	this->anchor_column = this->semi_width - map_col * this->zoom_level;
	this->anchor_line = this->semi_height - map_line * this->zoom_level;
}
