#include "Screen.hpp"

#include <iostream>


int Screen::load_main_menu_texture() noexcept {
	// 메인 메뉴에 필요한 버튼 설정 및 키 설정 불러오기

	std::clog << "\nLoading a main menu\n";

	// 버튼과 오브젝트들을 비워줌.
	this->clear_vectors();

	auto constexpr PIXEL_BUTTON = 32;

	auto dm = this->window.display_mode();

	sdl::Rect button_rect{ 30, 30, 150, 50 };
	//sdl::Rect button_rect{ dm.w * 2 / 3, dm.h / 3, 150, 50 }; // 원격접속하니 모니터 크기가 정확하지 않아서 임시로 둔다.

	this->v_button.emplace_back(std::make_unique< Button>(
		this->texture_button, this->font, this->renderer, u8"새 game", false, button_rect, PIXEL_BUTTON
		));

	button_rect.y += button_rect.h;

	this->v_button.emplace_back(std::make_unique< Button>(
		this->texture_button, this->font, this->renderer, u8"load game", false, button_rect, PIXEL_BUTTON
		));

	this->renderer.clear(this->background_color);
	this->should_redraw_button = true;

	return 0;
}



void Screen::handle_events_main(void) noexcept {

	switch (this->event_handler.type)
	{
	case SDL_QUIT:
		// 제일 처음에 와야 다른 마우스 케이스에 들어가지 않는다.
		this->menu_type = MENU_TYPE::QUIT;
		break;	// SDL_QUIT 

	case SDL_MOUSEMOTION:
		for (auto const& a : this->v_button) {
			// 마우스가 움직여서 버튼위에 위치하는지 확인함
			if (a->check_mouse(event_handler.motion.x, event_handler.motion.y, false)) {
				this->should_redraw_button = true;
				//break;
			}
		}

		break;	// SDL_MOUSEMOTION

	case SDL_MOUSEBUTTONDOWN:	// 마우스 버튼 눌렀을 때
		
		int mouse_col, mouse_line; // 마우스 픽셀 위치 저장
		SDL_GetMouseState(&mouse_col, &mouse_line);

		//		버튼 우선 확인		//

		// 누른 위치가 버튼인지 확인
		for (auto const& a : this->v_button) {
			if (a->check_mouse(mouse_col, mouse_line, true)) {
				this->menu_type = MENU_TYPE::IN_GAME;

				this->should_redraw_button = true;
				//break;
			}
		}

		break;	// SDL_MOUSEBUTTONDOWN


	case SDL_MOUSEBUTTONUP:		// 마우스 버튼 들었을 때

		for (auto const& a : this->v_button) {
			if (a->check_mouse(mouse_col, mouse_line, false)) {
				this->should_redraw_button = true;
				//break;
			}
		}


		break;	// SDL_MOUSEBUTTONUP


	default:
		break;
	} // this->me_event.type

}

