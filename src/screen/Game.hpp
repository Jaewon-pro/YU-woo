#pragma once

#include "Screen.hpp"
#include "map/MapManager.hpp"
#include "FileManager.hpp"
#include "entity/Entity.hpp"
#include "key_bind/Command.hpp"

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

	int update_game(int const game_speed) noexcept;

	int game_loop_main_menu(void) noexcept;

	int load_game_from_saved_file(std::string saved_file_path) noexcept;

	// screen //
	void draw_map(void) noexcept; // 일단 맵 1개로 실험

	void draw_sprites(void) noexcept;

	void draw_sprite_background(int const col, int const row) noexcept;
	void draw_sprite_background(std::pair<double, double> pos) noexcept;

};
