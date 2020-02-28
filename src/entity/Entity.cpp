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
	// ���� ���� ���� other�� dist=0 �Ÿ�(�簢��) ���� ��ġ�� �ִٸ� 0 ��ȯ
	// ���� �ۿ� �����ϸ� -1 ��ȯ
	double constexpr TOLERANCE = 1.0E-8; // ���� �Ѱ� ������

	if (std::abs(this->pos.first  - other_pos.first)  > TOLERANCE + dist) { return -1; }
	if (std::abs(this->pos.second - other_pos.second) > TOLERANCE + dist) { return -1; }

	return 0;
}

int Coordinate::compare(Coordinate const& other, double dist) const noexcept {

	return this->compare(other.pos);
}


// protected

void Coordinate::round(void) noexcept {
	// ��ǥ�� ���� �ݿø� ��Ŵ.
	// ���� ������ ����� ���� �����ϱ� ���� Ư�� ��ġ ������ �ݿø� ���Ѽ� ������ ���ش�.
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

	if (this->hp <= 0) { // Actor �����
		this->is_dead = true;

		this->l_command.clear(); // ����� �ൿ ����

		// �����۵��� �ʿ� ��Ѹ�
		// to do item ������ �Ҽ� ��ȯ
	}
}

int Actor::update_act(void) noexcept {
	// 1 ���� tick ���� �� �Լ��� ȣ���Ҳ���. �� �ϴ� �� �ൿ�̶�� �����ϼ�.
	// Actor �� �ൿ ����Ʈ���� �� ó�� �ൿ�� �õ��Ѵ�.
	
	if (this->l_command.size() < 1) return 1;

	if (Coordinate::compare( this->l_command.front().second, 1.0 ) == 0) {
		// ��ȣ�ۿ��� �� �� �ִ� �Ÿ���
		
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
			this->l_command.pop_front(); // ���������� �ൿ�� �Ϸ��ߴٸ� �� �ൿ�� ������.

		}

		//Coordinate::round(); // �ݺ��� ������� ���� �Ҽ��� ������ �ݿø����� ���ش�.
		return 0;
	}
	
	// ��ȣ�ۿ� ���ϴ� ���


	this->l_command.pop_front(); // ���������� �ൿ�� �Ϸ��ߴٸ� �� �ൿ�� ������.
	// �����Ҽ� ���� ����̾ ����

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

