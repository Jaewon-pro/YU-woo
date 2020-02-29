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
	// 줄을 마무리 하고 텍스쳐를 만들어서 리스트에 추가함. 줄을 마칠때 무조건 써줘야 한다.
	// is_append_mode가 true 이면 새로운 줄을 뒤에 추가 기본값

	if (this->buffer.compare(L"") == 0) {
		return;
	}

	int width_text = 0, h = 0;
	this->ref_font.size_text(this->buffer, width_text, h);

	// 버퍼의 문장이 너무 길면 공백이나 \n 등등을 기점으로 다음 문장으로 넘김
	while (this->text_boundary.w < width_text) {

		// 다음 줄로 넘기기 위한 기준을 찾기 위해 공백을 찾음
		// 어림잡아 자를 영역의 앞 부분에서 공백이 있는지 검색
		auto index_space = this->buffer.find(L' ', width_text / this->text_boundary.w);

		if (index_space == std::string::npos) {
			break;
		} // 공백도 없는 문장이면 그냥 경계에 짤리더라도 한줄인 텍스쳐로 만듦. 이 경우에는 방법이 생각이 안 난다.
		
		std::wstring wstr_tmp = this->buffer.substr(index_space + 1);

		// 공백 이전 까지만 텍스쳐로 만듦
		this->buffer = this->buffer.substr(0, index_space);
		this->make_texture_from_buffer();

		// 버퍼에 텍스쳐를 만든 텍스트 뒷부분으로 이동 대입
		this->buffer = std::move(wstr_tmp);

		width_text = 0, h = 0; // 폰트와 글자에 따라 텍스쳐 길이가 달라지므로 문장 길이 확인
		this->ref_font.size_text(this->buffer, width_text, h);
	}

	this->make_texture_from_buffer();

	this->buffer = L"";
}

void Message::draw(int margin) const noexcept {
	// margin = 0 은 좌우 공백 픽셀수
	int line_pixel = 0; // 1줄 기준 픽셀 크기

	for (auto const& itr_tex : this->l_message_texture) {
		// list 반복자 순서대로 boundary 내에서 문자열 텍스쳐 표시함.

		this->ref_render.render_copy( itr_tex,
			{ 0, 0, itr_tex.size().x, itr_tex.size().y },
			{ text_boundary.x + margin, text_boundary.y + line_pixel, itr_tex.size().x, itr_tex.size().y});

		line_pixel += itr_tex.size().y;

		if (this->text_boundary.h < line_pixel) {
			return; // boundary보다 줄 수가 많은 문장이라면 짜름. 이하는 표시 안함.
		}
	}
}

void Message::clear_list(int num) noexcept {
	// 만들어진 리스트의 텍스쳐들을 num=0 숫자가 될때까지 지움.
	while (this->l_message_texture.size() > num) {
		this->l_message_texture.pop_back();
	}
}


// private function

void Message::make_texture_from_buffer(void) noexcept {
	// 버퍼에 있는 wstring을 리스트 맨 뒤에 추가함.
	this->l_message_texture.emplace_back(
		sdl::Texture(this->ref_font.make_text_texture(this->buffer.c_str(), this->ref_render.ptr(), this->text_color)));
}
