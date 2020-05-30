#include "Game.hpp"

#include <iostream>
#include <memory>

Game::Game(const char* title, bool fullscreen, unsigned max_fps)
	: FRAME_DELAY{ 1000 / max_fps }
	, screen{ title, this->import_manage.get_size(), fullscreen, this->import_manage.get_font_path() }
	, map{ 1 }
{

	this->screen.message_log << u8"as한f";
	this->screen.message_log.end_line();
}

Game::~Game() {

}


int Game::game_loop(void) noexcept {
	// 메뉴 선택 등의 분기를 담당하는 함수

	while (this->screen.get_menu_type() != MENU_TYPE::QUIT)
	{
		switch (this->screen.get_menu_type())
		{
		case MENU_TYPE::MAIN_MENU:

			this->game_loop_main_menu();
			break;

		case MENU_TYPE::IN_GAME:

			this->game_loop_in_game();
			break;

		default:
			break;
		}
		sdl::Timer::delay(100);

	}

	return 0;
}


int Game::game_loop_in_game(void) noexcept {

	int game_speed = 1;

	this->load_game_from_saved_file("tmp");

	this->screen.load_ingame_texture();

	while (this->screen.get_menu_type() == MENU_TYPE::IN_GAME) {
		auto frame_start = sdl::Timer::ticks_u32();

		if (this->screen.get_redraw()) {
			//			GAME		//
			this->screen.clear_screen();
			this->draw_map();

			this->draw_sprites();

			//			UI			//
			this->screen.message_log.draw();

			this->screen.draw_buttons(); // 타일을 그리고 나면 버튼이 가릴수도 있으니 다시 그려줌.
			this->screen.end_redraw();
		}

		if (this->screen.get_redraw_button()) {
			this->screen.draw_buttons();
		}

		while (this->screen.get_event().poll() == true) {
			Command* command = this->screen.handle_events();
			
			if (command) {
				command->excute(this->map.actor_tmp(), this->screen.get_mouse_pos());
			}
		}

		this->update_game(game_speed);

		auto frame_time = sdl::Timer::ticks_u32() - frame_start;

		if (this->FRAME_DELAY > frame_time) {
			sdl::Timer::delay(this->FRAME_DELAY - frame_time);
		}
	}
	return 0;
}

int Game::update_game(int const game_speed) noexcept {
	// 게임 내적 요소와 프레임 마다 바뀌는 요소들을 업데이트 하는 함수

	if (game_speed < 1) return 1;

	// previous_time의 값을 통해 일정 시간에 도달해야지만 1 게임 Tick이 지나게 설정
	if ( (sdl::Timer::ticks_u32() - this->previous_time) < static_cast<uint32_t>(300 / game_speed) ) {
		return 2;
	} else {
		this->previous_time = sdl::Timer::ticks_u32();
	}

	// 게임 내부 로직 실행 //
	this->map.update();

	// 스크린 //
	this->screen.update();

	this->draw_sprites();

	return 0;
}


int Game::game_loop_main_menu(void) noexcept {
	// Main Menu
	this->screen.load_main_menu_texture();

	while (this->screen.get_menu_type() == MENU_TYPE::MAIN_MENU) {
		auto frame_start = SDL_GetTicks();

		if (this->screen.get_redraw_button()) {
			this->screen.draw_buttons();
		}

		// 이벤트가 일어날 때까지 프로그램은 대기.
		this->screen.get_event().wait();

		this->screen.handle_events_main();

		// Limit maximum fps.
		auto frame_time = SDL_GetTicks() - frame_start;

		// Delay game to reduce the cpu memory consum.
		if (FRAME_DELAY > frame_time) {
			sdl::Timer::delay(FRAME_DELAY - frame_time);
		}
	}
	return 0;
}

int Game::load_game_from_saved_file(std::string file_path) noexcept {

	this->map.load_from_saved_file(file_path);

	return 0;
}

// screen //

void Game::draw_map(void) noexcept {

	for (int row = 0; row < map.get_size_row(); ++row) {
		for (int col = 0; col < map.get_size_col(); ++col) {

			screen.draw_terrain(map.get_terrain_idx({ col, row }), col, row);
			screen.draw_block(map.get_block_idx({ col, row }), col, row);
			
		}
	}
}

void Game::draw_sprites(void) noexcept {
	// 게임의 모든 스프라이트를 그리는 함수
	for (auto const& actor : this->map.get_actor_list()) {

		this->draw_sprite_background(actor.get_pos());
		this->screen.draw_sprite(actor.get_pos());
	}
	
	this->screen.render();
}

void Game::draw_sprite_background(int const col, int const row) noexcept {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			// 주위 2x2에만 그림을 그림.
			if (this->map.is_out_of_map(col + i, row + j))
				continue;
			this->screen.draw_terrain(map.get_terrain_idx({ col+ i, row+ j }), col+ i, row+ j);
			this->screen.draw_block(map.get_block_idx({col+ i, row+ j }), col+ i, row+ j);
		}
	}
}

void Game::draw_sprite_background(std::pair<double, double> pos) noexcept {
	this->draw_sprite_background(static_cast<int>(pos.first), static_cast<int>(pos.second));
}
