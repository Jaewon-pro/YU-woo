#pragma once

#include <list>

class MapManager;
class Coordinate;

class Command
{
public:
	virtual ~Command() { }
	virtual void excute(MapManager& game_map, std::pair <double, double> pos) = 0;
};


class MoveCommand : public Command
{
public:
	virtual void excute(MapManager& map, std::pair <double, double>) noexcept;
};

class AttackCommand : public Command
{
public:
	virtual void excute(MapManager& map, std::pair <double, double> pos) noexcept;
};

class TestCommand : public Command
{
public:
	virtual void excute(MapManager& map, std::pair <double, double> pos) noexcept;
};
