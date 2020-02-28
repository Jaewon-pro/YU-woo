#pragma once

#include <string>
#include <fstream>

// 게임 외적인 설정 불러오고 저장하는 클래스
class FileManager
{
public:
	FileManager(void);
	~FileManager(void);

	std::string get_font_path(void) const;

	std::pair <int, int> get_size(void) const;

private:
	std::string			language;
	std::string			font_name;
	std::string			version;

	std::pair <int, int> screen_size;


private:
	int load_setting(void);

	inline int parse_txt(std::ifstream& file, std::string& str_data) noexcept;

	inline int save_setting2field(std::ifstream& file) noexcept;
};

