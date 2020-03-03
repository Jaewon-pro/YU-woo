#include "Screen.hpp"

#include <iostream>


Screen::Screen(const char* title, std::pair <int, int> const& screen_size, bool fullscreen, std::string const& font_path)
	: should_redraw{ true }, should_redraw_button{ true }, is_fullscreen{ fullscreen }
	, menu_type{ MENU_TYPE::MAIN_MENU }
	, window{ title, {screen_size.first, screen_size.second} }
	, background_color{ 0, 0, 0, 255 }
	, camera{ screen_size.first, screen_size.second }, message_log{ this->font, this->renderer }
{
	std::clog << "Success to make Window...\n";


	try {
		this->renderer = this->window.make_renderer();
		this->font = sdl::Font(font_path, 32);
	}

	catch (sdl::Exception const& e) {	// renderer가 생성되지 못했을 때
		std::cerr << "Fail to make renderer... in Screen::Screen init" << "\n";

		sdl::show_message_box(
			SDL_MESSAGEBOX_ERROR,
			"Fail to make Renderer!!",
			e.what(),
			this->window );

		exit(112);
	}

	catch (std::exception const& error ) {	// font 파일을 못 찾거나 등등의 오류
		std::cerr << error.what() << font_path << "\n";
		std::cerr << "In function: " << __FUNCDNAME__ << "\n";

		sdl::show_message_box(
			SDL_MESSAGEBOX_ERROR,
			"Fail to load Font",
			font_path,
			this->window );

		exit(113);
	}

	std::clog << "Success to make Renderer...\n";
	this->window.set_fullscreen(fullscreen);

	std::clog << "Success to load Font: " << font_path << "\n";

	const std::string path_button = "assets/button.png";
	int constexpr PIXEL_BUTTON = 32;

	this->texture_button = std::move(TextureManager(this->renderer, path_button, PIXEL_BUTTON));
}

Screen::~Screen() {
	this->key.clear();

	this->texture_button.~TextureManager();
	this->window.~Window();
	this->renderer.~Renderer();
	this->font.~Font();
}


int Screen::load_ingame_texture(void) noexcept {
	std::clog << "\nLoading a game\n";

	this->clear_vectors();


	this->key.load_setting();

	const std::string path_terrain = "assets/tile/tile.png";
	const std::string path_block = "assets/block/block.png";

	// png 파일에서 읽어올 한 이미지의 픽셀수
	// 여러 이미지가 한 png 파일에 포함되어 있다. 따라서 이를 구분 지어주는 크기
	int constexpr PIXEL_BLOCK = 64;

	this->v_object.emplace_back(std::make_unique< Object_tex>(this->renderer, path_terrain, PIXEL_BLOCK));
	this->v_object.emplace_back(std::make_unique< Object_tex>(this->renderer, path_block, PIXEL_BLOCK));

	// sprite 이미지
	const std::string path_sprite = "assets/player.png";
	this->v_sprite.emplace_back(std::make_unique< Sprite>(this->renderer, path_sprite, PIXEL_BLOCK, 10));

	// Button 생성

	sdl::Rect button_rect = { 0, 0, 150, 50 };

	std::list<std::wstring> menu_text{ L"한글", L"GO TO THE MAIN MENU" };

	for (auto const& text : menu_text) {
		int constexpr PIXEL_BUTTON = 32;

		this->v_button.emplace_back(std::make_unique< Button >(
			this->texture_button, this->font, this->renderer, text, false, button_rect, PIXEL_BUTTON
			));

		button_rect.x += button_rect.w;
	}

	this->should_redraw = true;
	this->should_redraw_button = true;

	return 0;
}


Command* Screen::handle_events(void) noexcept {
	// 단순 화면과 연관된 사용자 입력은 여기서 처리하고
	// 게임 내적과 연관된 사용자 입력은 class KeyBind, Command 에서 처리한다.
	
	switch (this->event_handler.type)
	{
	case SDL_QUIT:
		this->menu_type = MENU_TYPE::QUIT;
		break;

	case SDL_MOUSEMOTION:
		for (auto const& p_button : this->v_button) {
			// 마우스가 움직여서 버튼위에 위치하는지 확인함
			if (p_button->check_mouse(this->event_handler.motion.x, this->event_handler.motion.y, false)) {
				this->should_redraw_button = true;
			}
		}

		break;	// SDL_MOUSEMOTION

	case SDL_MOUSEBUTTONDOWN:

		int mouse_col, mouse_line;
		SDL_GetMouseState(&mouse_col, &mouse_line);

		//		버튼 우선 확인		//

		// 누른 위치가 버튼인지 확인
		for (auto const& p_button : this->v_button) {
			if (p_button->check_mouse(mouse_col, mouse_line, true)) {

				this->menu_type = MENU_TYPE::MAIN_MENU;

				this->should_redraw_button = true;
			}
		}
		if (should_redraw_button == true) break; // 버튼 관련만 하고 게임 내적 요소는 관련x
		

		//		게임내 요소 확인		//

		int map_col, map_line;

		if (this->camera.find_mouse_pos(mouse_col, mouse_line, map_col, map_line) == false)
			break;

		std::clog << map_col << " " << map_line << "\n";

		this->mouse_pos = { map_col, map_line };

		break;	// SDL_MOUSEBUTTONDOWN


	case SDL_MOUSEBUTTONUP:

		for (auto const& p_button : this->v_button) {
			if (p_button->check_mouse(mouse_col, mouse_line, false)) {
				this->should_redraw_button = true;
			}
		}
		
		break;

	case SDL_KEYDOWN:
		switch (this->event_handler.key.keysym.sym) 
		{
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
			// 카메라 이동은 화면 이동과 반대 방향
			this->camera.move_camera(
				static_cast<int>(this->event_handler.key.keysym.sym == SDLK_LEFT) - static_cast<int>(this->event_handler.key.keysym.sym == SDLK_RIGHT),
				static_cast<int>(this->event_handler.key.keysym.sym == SDLK_UP) - static_cast<int>(this->event_handler.key.keysym.sym == SDLK_DOWN)
			);
			
			this->should_redraw = true;

			break; // 키보드 화살표

		case SDLK_ESCAPE:
			this->menu_type = MENU_TYPE::QUIT;
			break;
			
		case SDLK_q:
			this->camera.jump_to_loc(13, 21);
			this->should_redraw = true;
			break;

		case SDLK_F11:
			// 전체화면 설정. 반전 시킴.
			this->is_fullscreen = !this->is_fullscreen;

			this->window.set_fullscreen(this->is_fullscreen);

			const SDL_DisplayMode dm = this->window.display_mode();

			// 화면 크기 변환후 카메라의 위치를 적당히 조절함.
			this->camera.set_screen_size(dm.w, dm.h);
			std::clog << dm.w << " " << dm.h << "\n";

			this->should_redraw = true;
			this->should_redraw_button = true;

			break;


		default:
			return this->key[this->event_handler.key.keysym.sym];
		}		// this->event_handler.key.keysym.sym
		break;	// SDL_KEYDOWN


	case SDL_MOUSEWHEEL:
		// 휠을 당기면 보이는 전체 이미지수는 많아지고 각각 이미지의 크기는 작아짐

		// me_event.wheel.y 이 반환하는 값은 -4 ~ +4
		// 세게 당길수록 커지나 대부분 1 아니면 -1 이다. 0은 반환 x
		this->camera.zoom_camera(this->event_handler.wheel.y);

		this->should_redraw = true;

		break; // SDL_MOSUEWHEEL


	default:
		break;
	} // this->event_handler.type
	return nullptr;
}


void Screen::clear_screen(void) const noexcept {
	this->renderer.clear(this->background_color);
}

void Screen::update(void) const noexcept {

	// sprite와 같은 애니메이션이 필요한 객체들은 반복문 동안 계속 변해야 하므로 여기서 update 시킨다.
	for (auto const& p_sprite : this->v_sprite) {
		p_sprite->update_animation();
	}
}


void Screen::render(void) const noexcept {

	this->renderer.present();
}

void Screen::end_redraw() noexcept {
	this->should_redraw = false;
}

void Screen::draw_terrain(const int index, const int col, const int line) const noexcept {

	this->v_object[0]->set_image_src_from_index(index);

	this->v_object[0]->draw(this->renderer, this->camera.get_rect_dst(col, line));

}

void Screen::draw_block(const int index, const int col, const int line) const noexcept {
	if (index < 1) return;	// block::void는 생략
	this->v_object[1]->set_image_src_from_index(index);

	this->v_object[1]->draw(this->renderer, this->camera.get_rect_dst(col, line));

}

void Screen::draw_buttons(void) noexcept {

	for (auto const& p_button : this->v_button) {
		p_button->draw(this->renderer);
	}

	this->renderer.present();
	this->should_redraw_button = false;
}

void Screen::draw_sprite(double col, double line) noexcept {
	this->v_sprite[0]->draw(this->renderer, this->camera.get_rect_dst(col, line));
}

void Screen::draw_sprite(std::pair<double, double> pos) noexcept {
	this->draw_sprite(pos.first, pos.second);
}


// private

void Screen::clear_vectors(void) noexcept {
	this->v_object.clear();
	this->v_object.shrink_to_fit();

	this->v_button.clear();
	this->v_button.shrink_to_fit();

	this->v_sprite.clear();
	this->v_sprite.shrink_to_fit();

	this->key.clear();
}

