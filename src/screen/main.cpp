#include "Game.hpp"

int main(int argc, char* argv[]) {

	(void)argc;
	(void)argv;

	// The following classes manages the lifetime of SDL declared resources RAII style

	auto root = sdl::Root(SDL_INIT_EVENTS);


	auto game1 = Game("Jisu's Jesus shit", false, 144);


	game1.game_loop();

	return 0;

	// Cleanup is done automatically

}
