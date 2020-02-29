#include "Message.hpp"


Message::Message(sdl::Font const& font, sdl::Renderer const& render) noexcept
	: ref_font{ font }, ref_render{ render },
	text_boundary{ 0, 50, 150, 1000 }
{
}

Message::~Message() {
	this->clear_list();
}


template<>
Message& Message::operator<< <std::string> (std::string text) noexcept {
	this->buffer += sdl::Font::string_to_wstring(text);
	return *this;
}

template<>
Message& Message::operator<< <std::wstring> (std::wstring wstr_text) noexcept {
	this->buffer += wstr_text;
	return *this;
}

template<>
Message& Message::operator<< <int> (int num) noexcept {
	this->buffer += std::to_wstring(num);
	return *this;
}



void Message::end_line(void) noexcept {
	// ���� ������ �ϰ� �ؽ��ĸ� ���� ����Ʈ�� �߰���. ���� ��ĥ�� ������ ����� �Ѵ�.
	// is_append_mode�� true �̸� ���ο� ���� �ڿ� �߰� �⺻��

	if (this->buffer.compare(L"") == 0) {
		return;
	}

	int width_text = 0, h = 0;
	this->ref_font.size_text(this->buffer, width_text, h);

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
		this->make_texture_from_buffer();

		// ���ۿ� �ؽ��ĸ� ���� �ؽ�Ʈ �޺κ����� �̵� ����
		this->buffer = std::move(wstr_tmp);

		width_text = 0, h = 0; // ��Ʈ�� ���ڿ� ���� �ؽ��� ���̰� �޶����Ƿ� ���� ���� Ȯ��
		this->ref_font.size_text(this->buffer, width_text, h);
	}

	this->make_texture_from_buffer();

	this->buffer = L"";
}

void Message::draw(int margin) const noexcept {
	// margin = 0 �� �¿� ���� �ȼ���
	int line_pixel = 0; // 1�� ���� �ȼ� ũ��

	for (auto const& itr_tex : this->l_message_texture) {
		// list �ݺ��� ������� boundary ������ ���ڿ� �ؽ��� ǥ����.

		this->ref_render.render_copy( itr_tex,
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
		this->l_message_texture.pop_back();
	}
}


// private function

void Message::make_texture_from_buffer(void) noexcept {
	// ���ۿ� �ִ� wstring�� ����Ʈ �� �ڿ� �߰���.
	this->l_message_texture.emplace_back(
		sdl::Texture(this->ref_font.make_text_texture(this->buffer.c_str(), this->ref_render.ptr(), this->text_color)));
}
