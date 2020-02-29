#pragma once

#include <vector>
#include <list>
#include <deque>
#include <string>

class Actor;

struct game_tag
{
	int mod_id; // 모드마다 부여된 고유 번호
	int index;  // 모드 제작자가 부여한 고유 번호
	std::wstring name;
};


struct Terrain
{
	game_tag tag;
	int movement_speed; // -1 은 통과 불과
	int transparency; // 0은 투명 100은 불투명
	int flammability; // 0은 안탐. 100은 잘탐.
	int layer; // 층
};

struct Block
{
	game_tag tag;
	int hp;
};


struct Locale_map
{
	int map_index;
	int col;
	int line;
	std::string seed;

	std::vector < std::vector <Block const*> >	v_block;
	std::vector < std::vector <Terrain const*> >	v_terrain;

	std::list < Actor > l_actor;

};

// 현재 겜의 맵들을 관리하는 클래스
class MapManager
{
public:
	MapManager(int mod_num) noexcept;

	~MapManager() noexcept;

	int make_locale_from_seed(int col, int line, std::string const& seed) noexcept;

	int load_from_saved_file(std::string const& path) noexcept;


	int get_block_idx(std::pair<int, int> const& pos) const noexcept;
	int get_terrain_idx(std::pair<int, int> const& pos) const noexcept;

	int get_size_row(void) const noexcept;
	int get_size_col(void) const noexcept;

	std::list<Actor> const& get_actor_list(void) const noexcept;
	
	Actor& actor_tmp(void) noexcept;

	bool is_out_of_map(int const& pos_col, int const& pos_line) const noexcept;

	void update(void) noexcept;

private:
	int mod_num;
	int current_index; // 현재 선택중인 로컬맵

	std::deque < Locale_map > locale;

	std::vector < Block >	v_block_info;
	std::vector < Terrain >	v_terrain_info;

private:
	void init_vec(void) noexcept;

	void generate_by_seed(int terrain_type, int const& seedB, int const& seedT) noexcept;


	int distance(std::pair<int, int> const&, std::pair<int, int> const&) const noexcept;

	bool find_way(std::pair<int, int> const& dst, std::pair<int, int> const& src) const noexcept;

};

