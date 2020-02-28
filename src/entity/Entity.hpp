#pragma once

#include <deque>
#include <list>
#include <string>

class Coordinate;
class Actor;
class Item;

//////////////////////////////////////////////////////
//				class Entity defintion

class Coordinate
{
public:
	Coordinate() noexcept;

	Coordinate(double col, double line) noexcept;
	Coordinate(std::pair<double, double> pos) noexcept;

	virtual ~Coordinate() noexcept;

	void set_pos(double const& col, double const& line) noexcept;

	void move_pos(double const& dCol, double const& dLine) noexcept;

	std::pair <double, double> get_pos(void) const noexcept { return this->pos; }

	int compare(std::pair <double, double> other_pos, double dist = 0) const noexcept;
	int compare(Coordinate const& other, double dist = 0) const noexcept;

protected:
	void round(void) noexcept;

private:
	std::pair <double, double> pos;
};


//////////////////////////////////////////////////////
//				class Actor defintion

enum class STATE : int {
	WAIT, SLEEP, MOVE, ATTAK,
};

class Actor :public Coordinate
{
public:
	//default constructor
	Actor() noexcept;

	Actor(std::pair<double, double> pos) noexcept;

	~Actor() noexcept;

	void set_name(std::wstring const& wstr_name) noexcept;
	

	std::wstring get_name(void) const noexcept { return this->name; }

	int update_act(void) noexcept;


	void modify_hp(int const& num) noexcept;

	void clear_act(void) noexcept;

	int append_action(STATE const&, Coordinate const&) noexcept;
	int append_action(STATE const&, std::pair <int, int>) noexcept;
	int append_action(STATE const&, std::pair <double, double>) noexcept;

	// int job_giver( // ai

private:
	int hp = 1;
	int age = 0;
	bool is_dead = false;

	double speed = 0.1; // 1 tick 에 움직일 수 있는 칸수
	STATE state = STATE::WAIT;

	std::wstring name;
	std::list<std::pair<STATE, std::pair <double, double>> > l_command;

};


