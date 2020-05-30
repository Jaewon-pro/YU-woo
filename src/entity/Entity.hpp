#pragma once

#include <deque>
#include <list>
#include <string>

class Coordinate;
class Actor;


class Coordinate
{
public:
	Coordinate(double col = 0.0, double line = 0.0) noexcept;
	Coordinate(std::pair<double, double> pos) noexcept;

	virtual ~Coordinate();

	void set_pos(double col, double line) noexcept;

	void move_pos(double dCol, double dLine) noexcept;

	void move_toward(double col, double line) noexcept;

	std::pair <double, double> get_pos(void) const noexcept { return this->pos; }

	int compare(std::pair <double, double> other_pos, double interval) const noexcept;
	int compare(Coordinate const& other, double interval) const noexcept;

protected:
	void round(void) noexcept;

private:
	std::pair <double, double> pos;
};



enum class STATE : int {
	WAIT, SLEEP, MOVE, ATTAK,
};

class Actor : public Coordinate
{
public:
	Actor(std::pair<double, double> pos = { 0.0, 0.0 }) noexcept;

	~Actor();

	void set_name(std::wstring const wstr_name) noexcept;
	

	std::wstring get_name(void) const noexcept { return this->name; }

	int update_act(void) noexcept;


	void modify_hp(int const num) noexcept;

	int append_action(STATE, std::pair <int, int>) noexcept;
	int append_action(STATE, std::pair <double, double>) noexcept;


private:
	int hp = 1;
	int age = 0;
	bool is_dead = false;

	double speed = 0.1; // 1 tick 에 움직일 수 있는 칸수
	STATE state = STATE::WAIT;

	std::wstring name;
	std::list<std::pair<STATE, std::pair <double, double>> > l_command;


private:
	void clear_command_list(void) noexcept;
};


