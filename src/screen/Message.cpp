#include "Message.hpp"

Message::Message(sdl::Font const& font, sdl::Renderer const& render) noexcept
	: rhs_font{ font }, rhs_render{ render },
	text_boundary{ 0, 50, 150, 1000 }
{
}

Message::~Message() {
	this->clear_list();
}

Message& Message::operator<<(std::string const& str_text) noexcept {
	// ����)) Message log; log << "test text";
	this->buffer += sdl::Font::string2wstr(str_text);
	return *this;
}

Message& Message::operator<<(std::wstring const& wstr_text) noexcept {
	this->buffer += wstr_text;
	return *this;
}

Message& Message::operator<<(int num) noexcept {
	this->buffer += std::to_wstring(num);
	return *this;
}


void Message::end_line(bool is_append_mode) noexcept {
	// ���� ������ �ϰ� �ؽ��ĸ� ���� ����Ʈ�� �߰���. ���� ��ĥ�� ������ ����� �Ѵ�.
	// is_append_mode�� true �̸� ���ο� ���� �ڿ� �߰� �⺻��

	if (this->buffer.compare(L"") == 0) {
		return;
	}

	int width_text = 0, h = 0; // ��Ʈ�� ���ڿ� ���� �ؽ��� ���̰� �޶����Ƿ� ���� ���� Ȯ��
	this->rhs_font.size_text(this->buffer, width_text, h);

	// ������ ������ �ʹ� ��� �����̳� \n ����� �������� ���� �������� �ѱ�
	while (this->text_boundary.w < width_text) {

		// ���� �ٷ� �ѱ�� ���� ������ ã�� ���� ������ ã��
		// ���� �ڸ� ������ �� �κп��� ������ �ִ��� �˻�
		auto index_space = this->buffer.find(L' ', width_text / this->text_boundary.w);

		if (index_space == std::string::npos) {
			break;
		} // ���鵵 ���� �����̸� �׳� ��迡 ©������ ������ �ؽ��ķ� ����. �� ��쿡�� ����� ������ �� ����.
		
		std::wstring wstr_tmp = this->buffer.substr(index_space + 1);

		// ���� ���� ������ �ؽ��ķ� ����
		this->buffer = this->buffer.substr(0, index_space);
		this->make_texture_from_buffer(is_append_mode);

		// ���ۿ� �ؽ��ĸ� ���� �ؽ�Ʈ �޺κ����� �̵� ����
		this->buffer = std::move(wstr_tmp);

		width_text = 0, h = 0; // ��Ʈ�� ���ڿ� ���� �ؽ��� ���̰� �޶����Ƿ� ���� ���� Ȯ��
		this->rhs_font.size_text(this->buffer, width_text, h);
	}

	this->make_texture_from_buffer(is_append_mode);

	this->buffer = L""; // ���� �ʱ�ȭ
}

void Message::draw(int margin) const noexcept {
	// margin = 0 �� �¿� ���� �ȼ���
	int line_pixel = 0; // 1�� ���� �ȼ� ũ��

	for (auto const& itr_tex : this->l_message_texture) {
		// list �ݺ��� ������� boundary ������ ���ڿ� �ؽ��� ǥ����.

		this->rhs_render.render_copy( itr_tex,
			{ 0, 0, itr_tex.size().x, itr_tex.size().y },
			{ text_boundary.x + margin, text_boundary.y + line_pixel, itr_tex.size().x, itr_tex.size().y});

		line_pixel += itr_tex.size().y;

		if (this->text_boundary.h < line_pixel) {
			return; // boundary���� �� ���� ���� �����̶�� ¥��. ���ϴ� ǥ�� ����.
		}
	}
}

void Message::clear_list(int num) noexcept {
	// ������� ����Ʈ�� �ؽ��ĵ��� num=0 ���ڰ� �ɶ����� ����.
	while (this->l_message_texture.size() > num) {
		this->l_message_texture.pop_back(); // �ں��� ����
	}
}


// private function

void Message::make_texture_from_buffer(bool is_append_mode) noexcept {
	// ���ۿ� �ִ� wstring�� ����Ʈ �� �ڿ� �߰���.
	if (is_append_mode) {
		this->l_message_texture.emplace_back(
			sdl::Texture(this->rhs_font.make_text_texture(this->buffer.c_str(), this->rhs_render.ptr(), this->text_color)));
	}
	// ���ۿ� �ִ� wstring�� ����Ʈ �� �տ� �߰���.
	else {
		this->l_message_texture.emplace_front(
			sdl::Texture(this->rhs_font.make_text_texture(this->buffer.c_str(), this->rhs_render.ptr(), this->text_color)));
	}
}
