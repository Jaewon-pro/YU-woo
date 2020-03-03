#pragma once

#define CPP_SDL2_USE_SDL_IMAGE
#include <cpp-sdl2/sdl.hpp>

#include "ObjectManager.hpp"
#include "Button.hpp"
#include "Camera.hpp"
#include "Message.hpp"
#include "key_bind/KeyBind.hpp"

enum class MENU_TYPE : int {
	MAIN_MENU, IN_GAME, QUIT
};


class Screen
{
public:
	Screen(const char* title, std::pair <int, int> const& screen_size,
		bool is_fullscreen, std::string const& font_path
	);


	~Screen();

	// Load
	int load_ingame_texture(void) noexcept;


	// in - game
	Command* handle_events(void) noexcept;

	void update(void) const noexcept;

	void render(void) const noexcept;
	void end_redraw(void) noexcept;

	// get
	MENU_TYPE get_menu_type(void) const noexcept { return this->menu_type; }

	bool get_redraw(void) const noexcept { return this->should_redraw; }
	bool get_redraw_button(void) const noexcept { return this->should_redraw_button; }

	sdl::Event& get_event() noexcept { return this->event_handler; }

	std::pair<int, int> get_mouse_pos() const noexcept { return this->mouse_pos; }

	void clear_screen(void) const noexcept;

	// draw
	void draw_terrain(const int index, const int col, const int line) const noexcept;
	void draw_block(const int index, const int col, const int line) const noexcept;

	void draw_buttons(void) noexcept;

	void draw_sprite(double col, double line) noexcept;
	void draw_sprite(std::pair<double, double> pos) noexcept;


	// Main menu
	int load_main_menu_texture(void) noexcept;

	void handle_events_main(void) noexcept;

public:
	Message			message_log;

private:
	bool			should_redraw;		// 변경으로 다시 그려야 하는지
	bool			should_redraw_button;// 변경으로 다시 그려야 하는지

	bool			is_fullscreen;

	MENU_TYPE		menu_type;		// 게임이 수행할 타입 선택

	sdl::Window		window;			// 윈도우 창을 띄움
	sdl::Renderer	renderer;
	sdl::Event		event_handler;		// 이벤트 처리
	sdl::Color		background_color;	// 배경 색

	std::vector <std::unique_ptr <Object_tex>> v_object;
	std::vector <std::unique_ptr <Button>> v_button;

	std::vector <std::unique_ptr <Sprite>> v_sprite;

	TextureManager	texture_button;

	Camera			camera;
	sdl::Font		font;

	std::pair<int, int> mouse_pos{0, 0};

	KeyBind			key;

private:
	void clear_vectors(void) noexcept;

};


