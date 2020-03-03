#include "KeyBind.hpp" // class Key

#include <SDL_keycode.h> // SDLK


KeyBind::~KeyBind() noexcept {
	this->clear();
}

int KeyBind::load_setting(void) noexcept {
	// 미리 저장된 사용자 지정 키를 json 등으로 불러와 알맞게 저장

	this->key.emplace(SDLK_a, &this->move);

	return 0;
}

void KeyBind::swap_key(int dst_key, int src_key) noexcept {

}

void KeyBind::clear(void) noexcept {
	this->key.clear();
}


Command* KeyBind::operator[](int const sdl_keycode) noexcept {
	return this->key[sdl_keycode];
}
