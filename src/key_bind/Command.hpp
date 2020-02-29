#pragma once

#include <list>

class Actor;
class Coordinate;

class Command
{
public:
	virtual ~Command() { }
	virtual void excute(Actor& actor, std::pair <double, double> pos) = 0;
};


class MoveCommand : public Command
{
public:
	virtual void excute(Actor& actor, std::pair <double, double>) noexcept;
};

class AttackCommand : public Command
{
public:
	virtual void excute(Actor& actor, std::pair <double, double> pos) noexcept;
};

class TestCommand : public Command
{
public:
	virtual void excute(Actor& actor, std::pair <double, double> pos) noexcept;
};
