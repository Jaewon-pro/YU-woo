#include "FileManager.hpp"


FileManager::FileManager(void) noexcept {
	this->load_setting();
}

FileManager::~FileManager(void) {

}


std::string FileManager::get_font_path(void) const noexcept {
	// 폰트가 위치한 경로를 알맞게 맞춰서 넘겨줌
	return "data\\fonts\\" + this->language + "\\" + font_name;
}


std::pair <int, int> FileManager::get_size(void) const noexcept {
	// 게임 창 사이즈 넘겨줌
	return this->screen_size;
}


int FileManager::load_setting(void) noexcept {
	// 버전, 기본 세팅(화면 크기, 모드 목록)
	// 

	std::string		setting_file_name("setting.txt");

	std::ifstream	fin(setting_file_name);


	if (!fin.is_open()) {
		// setting.txt 열기 실패
		// 따라서 세팅 설정함.
		std::ofstream fout(setting_file_name);

		// 언어, 폰트
		fout << "Language=english\n";
		fout << "Font=fixedsys.ttf\n";


		// 게임
		fout << "Width=800\n";
		fout << "Height=600\n";

		fout.close();

		// 새로 만든 파일을 이제 멤버변수에 저장하자.
		this->load_setting();


		return 1;
	} // !fin.is_open()


	// txt에서 파싱한 데이터를 멤버변수에 저장
	this->save_setting2field(fin);
	fin.close();

	// 게임 버전 확인
	fin = std::ifstream("version.txt");

	if (!fin.is_open()) {
		// version.txt가 없는 경우, 새로 만듦
		std::ofstream fout("version.txt");

		fout << "1.0.0";

		fout.close();

		this->version = "1.0.0";
	} // ! open "version.txt"


	std::string	str;

	// 버전 정보 불러옴
	std::getline(fin, str);

	this->version = str;

	fin.close();

	return 0;
}

int FileManager::parse_txt(std::ifstream& file, std::string& str_data) noexcept {
	// 텍스트 파일에서 '=' 다음 문자를 파싱해서 데이터 저장함.

	std::getline(file, str_data);
	str_data = str_data.substr(str_data.find('=') + 1);

	return 0;
}

int FileManager::save_setting2field(std::ifstream& file_in) noexcept {
	// txt에서 파싱한 데이터를 멤버변수에 저장
	// 텍스트 파일에서 순서가 바뀌면 안 된다..

	// 언어, 폰트
	this->parse_txt(file_in, this->language);
	this->parse_txt(file_in, this->font_name);

	std::string tmp;

	// 스크린 사이즈
	this->parse_txt(file_in, tmp);
	this->screen_size.first = std::stoi(tmp);

	this->parse_txt(file_in, tmp);
	this->screen_size.second = std::stoi(tmp);

	return 0;
}
