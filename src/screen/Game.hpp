#pragma once

#include "Screen.hpp"
#include "src_ingame/MapManager.hpp"
#include "FileManager.hpp"
#include "src_ingame/Entity.hpp"
#include "src_ingame/Command.hpp"

class Game
{
public:
	Game(const char* title, bool fullscreen, unsigned fps);

	~Game();

	int game_loop(void) noexcept;

private:

	const uint32_t	FRAME_DELAY;

	FileManager		import_manage;
	MapManager		map;
	Screen			screen;

	uint32_t		previous_time = 0;



private:
	// logic //
	int game_loop_in_game(void) noexcept;

	int update_game(int const& game_speed) noexcept;

	void set_map(int const& size_col, int const& size_line, std::string const& seed) noexcept;

	int game_loop_main_menu(void) noexcept;

	int load_game_from_saved_file(std::string saved_file_path) noexcept;

	// screen //
	void draw_map(void) noexcept; // �ϴ� �� 1���� ����

	void draw_sprites(void) noexcept;

	void draw_sprite_background(int const& col, int const& row) noexcept;
	void draw_sprite_background(std::pair<double, double> pos) noexcept;

};
