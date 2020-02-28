#include "Command.hpp"
#include "MapManager.hpp"
#include "Entity.hpp"

void MoveCommand::excute(MapManager& map_, std::pair <double, double> pos) noexcept {
	//map_.get_selected_actor().append_actor(STATE::MOVE, pos);
}


void AttackCommand::excute(MapManager& map_, std::pair <double, double> pos) noexcept {

}

void TestCommand::excute(MapManager& map_, std::pair <double, double> pos) noexcept {
	
}
