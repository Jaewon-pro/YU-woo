#include <cmath>
#include <iostream>

#include "Entity.hpp"


Coordinate::Coordinate() noexcept
	: pos(0.0, 0.0)
{ }

Coordinate::Coordinate(double col, double line) noexcept
	: pos(col, line)
{ }

Coordinate::Coordinate(std::pair<double, double> pos) noexcept
	: pos(pos)
{ }


Coordinate::~Coordinate() noexcept {

}

// public

void Coordinate::set_pos(double const& col, double const& line) noexcept {
	this->pos = { col, line };
}

void Coordinate::move_pos(double const& dcol, double const& dline) noexcept {
	this->pos.first += dcol;
	this->pos.second += dline;
	std::clog << this->pos.first << " : x, " << this->pos.second << " : y Coord move\n";

}

int Coordinate::compare(std::pair <double, double> other_pos, double dist) const noexcept {
	// 오차 범위 내의 other와 dist=0 거리(사각형) 내에 위치에 있다면 0 반환
	// 범위 밖에 존재하면 -1 반환
	double constexpr TOLERANCE = 1.0E-8; // 절대 한계 오차값

	if (std::abs(this->pos.first  - other_pos.first)  > TOLERANCE + dist) { return -1; }
	if (std::abs(this->pos.second - other_pos.second) > TOLERANCE + dist) { return -1; }

	return 0;
}

int Coordinate::compare(Coordinate const& other, double dist) const noexcept {

	return this->compare(other.pos);
}


// protected

void Coordinate::round(void) noexcept {
	// 좌표를 각각 반올림 시킴.
	// 점점 오차가 생기는 것을 방지하기 위해 특정 위치 도착시 반올림 시켜서 오차를 없앤다.
	this->pos.first = std::round(this->pos.first);
	this->pos.second = std::round(this->pos.second);
}


Actor::Actor() noexcept
	: Coordinate{ 0.0, 0.0 }
{ }



Actor::Actor(std::pair<double, double> pos) noexcept
	: Coordinate(pos)
{ }

Actor::~Actor() noexcept {
	l_command.clear();
}

void Actor::set_name(std::wstring const& wstr_name) noexcept {
	this->name = wstr_name;
}

void Actor::modify_hp(int const& num) noexcept {
	this->hp += num;

	if (this->hp <= 0) { // Actor 사망시
		this->is_dead = true;

		this->l_command.clear(); // 예약된 행동 삭제

		// 아이템들을 맵에 흩뿌림
		// to do item 맵으로 소속 전환
	}
}

int Actor::update_act(void) noexcept {
	// 1 게임 tick 마다 이 함수를 호출할꺼임. 한 턴당 할 행동이라고 생각하셈.
	// Actor 의 행동 리스트에서 맨 처음 행동을 시도한다.
	
	if (this->l_command.size() < 1) return 1;

	if (Coordinate::compare( this->l_command.front().second, 1.0 ) == 0) {
		// 상호작용을 할 수 있는 거리임
		
		// do interact
		switch (this->l_command.front().first)
		{
		case STATE::WAIT:
			break;
		case STATE::SLEEP:
			break;
		case STATE::MOVE:
			Coordinate::move_pos(this->speed, 0.0);
			break;
		case STATE::ATTAK:
			break;

		default:
			break;
		}

		if (Coordinate::compare(this->l_command.front().second, 0.0) == 0) {
			this->l_command.pop_front(); // 성공적으로 행동을 완료했다면 이 행동을 삭제함.

		}

		//Coordinate::round(); // 반복된 계산으로 생긴 소수점 오차를 반올림으로 없앤다.
		return 0;
	}
	
	// 상호작용 못하는 경우


	this->l_command.pop_front(); // 성공적으로 행동을 완료했다면 이 행동을 삭제함.
	// 수행할수 없는 명령이어도 삭제

	return 2;
}

int Actor::append_action(STATE const& state, Coordinate const& target) noexcept {

	return this->append_action(state, target.get_pos());
}

int Actor::append_action(STATE const& state, std::pair <int, int> pos) noexcept {
	
	return this->append_action(state, std::pair( static_cast<double>(pos.first), static_cast<double>(pos.second) ));
}

int Actor::append_action(STATE const& state, std::pair <double, double> pos) noexcept {

	int constexpr MAX_ACT_LIST_NUM{ 50 };

	if (this->l_command.size() > MAX_ACT_LIST_NUM) {
		return 1;
	}

	this->l_command.emplace_back(state, pos);

	std::clog << "append action\n";
	return 0;
}

