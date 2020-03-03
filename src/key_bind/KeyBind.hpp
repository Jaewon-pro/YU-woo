#pragma once

#include <map>

#include "Command.hpp"


class KeyBind
{
public:
	~KeyBind() noexcept;
	int load_setting(void) noexcept;

	void swap_key(int dst, int src) noexcept;

	void clear(void) noexcept;

	Command* operator[](int const keycode) noexcept;

private:
	std::map < int, Command* > key; // SDLK, Command*
	MoveCommand   move;
	AttackCommand attack;
	TestCommand   test;
};
