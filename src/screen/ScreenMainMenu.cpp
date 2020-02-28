#include "Screen.hpp"

#include <iostream>


/////////////////////////////////////////////////////////////////////
//					Public funtion



int Screen::load_main_menu_texture() noexcept {
	// ���� �޴��� �ʿ��� ��ư ���� �� Ű ���� �ҷ�����

	std::clog << "\nLoading a main menu\n";

	// ��ư�� ������Ʈ���� �����.
	this->clear_vectors();

	auto constexpr PIXEL_BUTTON = 32;

	auto dm = this->window.display_mode();

	sdl::Rect button_rect{ dm.w * 2 / 3, dm.h / 3, 150, 50 };

	this->v_button.emplace_back(std::make_unique<Button>(
		this->texture_button, this->font, this->renderer, L"�� game", false,
		button_rect, PIXEL_BUTTON
		));

	button_rect.y += button_rect.h;

	this->v_button.emplace_back(std::make_unique<Button>(
		this->texture_button, this->font, this->renderer, "load game", false,
		button_rect, PIXEL_BUTTON
		));

	renderer.clear(this->background_color);
	this->should_redraw_button = true;

	return 0;
}



void Screen::handle_events_main(void) noexcept {

	switch (this->event_handler.type)
	{
	case SDL_QUIT:
		// ���� ó���� �;� �ٸ� ���콺 ���̽��� ���� �ʴ´�.
		this->menu_type = MENU_TYPE::QUIT;
		break;	// SDL_QUIT 

	case SDL_MOUSEMOTION:
		for (auto const& a : this->v_button) {
			// ���콺�� �������� ��ư���� ��ġ�ϴ��� Ȯ����
			if (a->check_mouse(event_handler.motion.x, event_handler.motion.y, false)) {
				this->should_redraw_button = true;
				//break;
			}
		}

		break;	// SDL_MOUSEMOTION

	case SDL_MOUSEBUTTONDOWN:	// ���콺 ��ư ������ ��
		// ���콺 �ȼ� ��ġ ����
		int mouse_col, mouse_line;
		SDL_GetMouseState(&mouse_col, &mouse_line);

		//		��ư �켱 Ȯ��		//

		// ���� ��ġ�� ��ư���� Ȯ��
		for (auto const& a : this->v_button) {
			if (a->check_mouse(mouse_col, mouse_line, true)) {
				this->menu_type = MENU_TYPE::IN_GAME;

				
				this->should_redraw_button = true;
				//break;
			}
		}

		break;	// SDL_MOUSEBUTTONDOWN


	case SDL_MOUSEBUTTONUP:		// ���콺 ��ư ����� ��

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

