#include "MapManager.hpp"
#include "entity/Entity.hpp"

#include <random>


MapManager::MapManager(int mod_num) noexcept
	: mod_num{ mod_num }
	, current_index{ 0 }
{
	// json 등등 파일 열기

	int mod_id = 0, index = 0;

	for (int n = 0; n < 6; ++n) {

		this->v_terrain_info.emplace_back( Terrain {
			.tag {.mod_id{ mod_id }, .index{ index }, .name{ L"void", } },
			.movement_speed{ -1 }, .transparency{ 0 }, .flammability{ 0 }, .layer{ 0 },
		} );

		++index;
	}

	index = 0;
	for (int n = 0; n < 8; ++n) {

		this->v_block_info.emplace_back( Block {
			 .tag {.mod_id{ mod_id }, .index{ index }, .name{ L"air", } },
			.hp{ 10 }
		} );

		++index;
	}
}


MapManager::~MapManager() {
	this->v_terrain_info.clear();
	this->v_terrain_info.shrink_to_fit();
	
	this->locale[current_index].l_actor.clear();
}



int MapManager::make_locale_from_seed(int col, int line, std::string const& str_seed) noexcept {

	int map_number = 0;

	if (this->locale.size() != 0) {
		map_number = this->locale.back().map_index + 1;
	}

	this->locale.emplace_back( Locale_map {
		.map_index = map_number, .col = col, .line = line, .seed = str_seed
	} );

	this->current_index = map_number;
	this->init_vec();

	this->generate_by_seed(123123, 45, 37);


	return 0;
}



int MapManager::load_from_saved_file(std::string const& path) noexcept {

	int map_number = 0;

	if (this->locale.size() != 0) {
		map_number = this->locale.back().map_index + 1;
	}
	
	// from the saved file.
	int col = 50;
	int line = 40;
	std::string seed = "asdf";

	this->locale.emplace_back(Locale_map{
		.map_index = map_number, .col = col, .line = line, .seed = seed
	});

	this->init_vec();

	// 저장된 값 대입 나중에 저장된 값을 불러와서 for문으로 일일이 대입하기
	this->generate_by_seed(1, 45, 37);

	this->locale[current_index].l_actor.emplace_back(Actor({ 1.25, 10.7 }));
	this->locale[current_index].l_actor.emplace_back(Actor({ 12.25, 4.1 }));
	this->locale[current_index].l_actor.emplace_back(Actor({ 5.25, 5.1 }));

	return 0;
}


int MapManager::get_block_idx(std::pair<int, int> const& pos) const noexcept { // col, line
	return this->locale[this->current_index].v_block[pos.second][pos.first]->tag.index;
}

int MapManager::get_terrain_idx(std::pair<int, int> const& pos) const noexcept {
	return this->locale[this->current_index].v_terrain[pos.second][pos.first]->tag.index;
}

int MapManager::get_size_row(void) const noexcept {
	return this->locale[this->current_index].line;
}

int MapManager::get_size_col(void) const noexcept {
	return this->locale[this->current_index].col;
}

std::list<Actor> const& MapManager::get_actor_list(void) const noexcept {
	return this->locale[this->current_index].l_actor;
}

Actor& MapManager::actor_tmp(void) noexcept {
	return this->locale[this->current_index].l_actor.front();
}

bool MapManager::is_out_of_map(int const& col, int const& line) const noexcept {
	if (col <= -1  || col  >= this->locale[this->current_index].col)  return true;
	if (line <= -1 || line >= this->locale[this->current_index].line) return true;
	return false;
}

void MapManager::update(void) noexcept {
	for (auto& actor : this->locale[current_index].l_actor) {
		actor.update_act();
	}
}


// private


void MapManager::init_vec(void) noexcept {
	// out of index 에러 방지를 위해서

	Locale_map& locale_map = this->locale[this->current_index];

	int line = locale_map.line;
	int col = locale_map.col;

	locale_map.v_block.resize(line);
	for (int i = 0; i < line; ++i) {
		locale_map.v_block[i].resize(col);
	}

	locale_map.v_terrain.resize(line);
	for (int i = 0; i < line; ++i) {
		locale_map.v_terrain[i].resize(col);
	}
}


int MapManager::distance(std::pair<int, int> const& a, std::pair<int, int> const& b) const noexcept {
	return abs(a.first - b.first) + abs(a.second - b.second);
}



void MapManager::generate_by_seed(int terrain_type, int const& seedB, int const& seedT) noexcept {
	
	Locale_map& locale_map = this->locale[this->current_index];

	int rand_seed = 1;
	{
		int num = 0;
		for (auto const& ch : locale_map.seed) {
			rand_seed += ch * ++num;
		}
		rand_seed = std::abs(rand_seed);
	}
	
	std::mt19937 engine_rand(rand_seed);

	const std::uniform_int_distribution<int> range(1, 100);

	Terrain const& VOID_ = this->v_terrain_info[0];
	Terrain const& SOIL = this->v_terrain_info[1];
	Terrain const& STONE = this->v_terrain_info[2];
	Terrain const& WATER = this->v_terrain_info[3];
	Terrain const& WATER_DEEP = this->v_terrain_info[4];
	Terrain const& SAND = this->v_terrain_info[5];


	for (int row = 0; row < locale_map.line; ++row) {
		for (int col = 0; col < locale_map.col; ++col) {

			if (range(engine_rand) <= seedT) {
				locale_map.v_terrain[row][col] = &SOIL;
			} else {
				locale_map.v_terrain[row][col] = &VOID_;
			}

			if (range(engine_rand) <= 20) {
				locale_map.v_terrain[row][col] = &WATER;
			}
		}
	}


	for (int repeat = 0; repeat < 4; ++repeat)
	{

	for (int row = 0; row < locale_map.line; ++row) {
		for (int col = 0; col < locale_map.col; ++col) {
			int cnt_terrain_major = 0, cnt_terrain_water = 0;

			for (int x = -1; x <= 1; ++x) {		// 주변 9칸에 있는지 확인
				for (int y = -1; y <= 1; ++y) {
					if (x == row && y == col) continue;	// 자기 자신은 제외
					if (this->is_out_of_map(col + y, row + x)) continue; // out of index 에러 방지

					if (locale_map.v_terrain[row + x][col + y] == &SOIL)       ++cnt_terrain_major;
					if (locale_map.v_terrain[row + x][col + y] == &WATER)      ++cnt_terrain_water;
					if (locale_map.v_terrain[row + x][col + y] == &WATER_DEEP) ++cnt_terrain_water;

				}// ++y
			}// ++x

			if (locale_map.v_terrain[row][col] == &SOIL && 3 <= cnt_terrain_water)
				locale_map.v_terrain[row][col] = &WATER;

			if (locale_map.v_terrain[row][col] == &WATER && cnt_terrain_water <= 3)
				locale_map.v_terrain[row][col] = &SOIL;

			if (locale_map.v_terrain[row][col] == &WATER && 8 <= cnt_terrain_water)
				locale_map.v_terrain[row][col] = &WATER_DEEP;

			if (locale_map.v_terrain[row][col] == &SOIL && cnt_terrain_major <= 2)
				locale_map.v_terrain[row][col] = &VOID_;

			if (locale_map.v_terrain[row][col] == &SAND && 3 <= cnt_terrain_major)
				locale_map.v_terrain[row][col] = &SOIL;

			if (locale_map.v_terrain[row][col] == &VOID_ && 4 <= cnt_terrain_major)
				locale_map.v_terrain[row][col] = &SAND;
		}// ++col
	}// ++row

	}// ++n


	Block const& ROCK = this->v_block_info[1];
	Block const& WATER_BLOCK = this->v_block_info[3];

	// 지형에 맞는 블럭 생성
	for (int row = 0; row < locale_map.line; ++row) {
		for (int col = 0; col < locale_map.col; ++col) {

			if (locale_map.v_terrain[row][col] == &VOID_) {
				locale_map.v_terrain[row][col] = &STONE;
			}
			if (locale_map.v_terrain[row][col] == &STONE) {
				locale_map.v_block[row][col] = &ROCK;
			}
			if (locale_map.v_terrain[row][col] == &WATER) {
				locale_map.v_block[row][col] = &WATER_BLOCK;
			}

			if (locale_map.v_terrain[row][col] == &SOIL) {
				if (range(engine_rand) <= seedB) {
					locale_map.v_block[row][col] = &this->v_block_info[4 + (rand() % 2)];
				}
				else if (range(engine_rand) <= (seedB - 30)) {
					locale_map.v_block[row][col] = &this->v_block_info[6 + (rand() % 2)];
				}
			}

			if (locale_map.v_block[row][col] == nullptr) {
				locale_map.v_block[row][col] = &this->v_block_info[0];
			}
		}
	}

}


bool MapManager::find_way(std::pair<int, int> const dst, std::pair<int, int> const src) const noexcept {

	// https://zerowidth.com/2013/a-visual-explanation-of-jump-point-search.html
	// https://joonleestudio.tistory.com/28
	// jump point search

	std::list < std::pair<int, int> > node;

	node.emplace_back(src);




	return true;
}

