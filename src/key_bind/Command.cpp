#include "Command.hpp"
#include "map/MapManager.hpp"
#include "entity/Entity.hpp"

void MoveCommand::excute(Actor& actor, std::pair <double, double> pos) noexcept {
	actor.append_action(STATE::MOVE, pos);
}


void AttackCommand::excute(Actor& actor, std::pair <double, double> pos) noexcept {

}

void TestCommand::excute(Actor& actor, std::pair <double, double> pos) noexcept {
	
}
