#pragma once

#include <map>			// bind keys

#include "Command.hpp"	// class Command


class KeyBind
{
public:
	~KeyBind() noexcept;
	int load_setting(void) noexcept;

	void swap_key(int dst, int src) noexcept;

	void clear(void) noexcept;

	Command* operator[](int keycode) const noexcept;

private:
	std::map<int, Command*> key; // SDLK, Command
	MoveCommand move;
	AttackCommand attack;
	TestCommand test;
};
