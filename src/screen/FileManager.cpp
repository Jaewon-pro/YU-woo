#include "FileManager.hpp"


FileManager::FileManager(void) noexcept {
	this->load_setting();
}

FileManager::~FileManager(void) {

}


std::string FileManager::get_font_path(void) const noexcept {
	// ��Ʈ�� ��ġ�� ��θ� �˸°� ���缭 �Ѱ���
	return "data\\fonts\\" + this->language + "\\" + font_name;
}


std::pair <int, int> FileManager::get_size(void) const noexcept {
	// ���� â ������ �Ѱ���
	return this->screen_size;
}


int FileManager::load_setting(void) noexcept {
	// ����, �⺻ ����(ȭ�� ũ��, ��� ���)
	// 

	std::string		setting_file_name("setting.txt");

	std::ifstream	fin(setting_file_name);


	if (!fin.is_open()) {
		// setting.txt ���� ����
		// ���� ���� ������.
		std::ofstream fout(setting_file_name);

		// ���, ��Ʈ
		fout << "Language=english\n";
		fout << "Font=fixedsys.ttf\n";


		// ����
		fout << "Width=800\n";
		fout << "Height=600\n";

		fout.close();

		// ���� ���� ������ ���� ��������� ��������.
		this->load_setting();


		return 1;
	} // !fin.is_open()


	// txt���� �Ľ��� �����͸� ��������� ����
	this->save_setting2field(fin);
	fin.close();

	// ���� ���� Ȯ��
	fin = std::ifstream("version.txt");

	if (!fin.is_open()) {
		// version.txt�� ���� ���, ���� ����
		std::ofstream fout("version.txt");

		fout << "1.0.0";

		fout.close();

		this->version = "1.0.0";
	} // ! open "version.txt"


	std::string	str;

	// ���� ���� �ҷ���
	std::getline(fin, str);

	this->version = str;

	fin.close();

	return 0;
}

int FileManager::parse_txt(std::ifstream& file, std::string& str_data) noexcept {
	// �ؽ�Ʈ ���Ͽ��� '=' ���� ���ڸ� �Ľ��ؼ� ������ ������.

	std::getline(file, str_data);
	str_data = str_data.substr(str_data.find('=') + 1);

	return 0;
}

int FileManager::save_setting2field(std::ifstream& file_in) noexcept {
	// txt���� �Ľ��� �����͸� ��������� ����
	// �ؽ�Ʈ ���Ͽ��� ������ �ٲ�� �� �ȴ�..

	// ���, ��Ʈ
	this->parse_txt(file_in, this->language);
	this->parse_txt(file_in, this->font_name);

	std::string tmp;

	// ��ũ�� ������
	this->parse_txt(file_in, tmp);
	this->screen_size.first = std::stoi(tmp);

	this->parse_txt(file_in, tmp);
	this->screen_size.second = std::stoi(tmp);

	return 0;
}
