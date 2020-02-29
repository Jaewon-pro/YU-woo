#pragma once

#include <string>
#include <fstream>

// ���� ������ ���� �ҷ����� �����ϴ� Ŭ����
class FileManager
{
public:
	FileManager(void) noexcept;
	~FileManager(void);

	std::string get_font_path(void) const noexcept;

	std::pair <int, int> get_size(void) const noexcept;

private:
	std::string			language;
	std::string			font_name;
	std::string			version;

	std::pair <int, int> screen_size;


private:
	int load_setting(void) noexcept;

	inline int parse_txt(std::ifstream& file, std::string& str_data) noexcept;

	inline int save_setting2field(std::ifstream& file) noexcept;
};

