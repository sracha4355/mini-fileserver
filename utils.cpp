#include <iostream>
#include <filesystem>
#include <vector>

namespace utils {

struct file_data {
	std::string path;
	std::string filename;
	uintmax_t filesize;
	file_data(const std::string& _path, const std::string& _filename, const uintmax_t& _size)
	:path(_path), filename(_filename), filesize(_size){}
	file_data(){}
};

std::vector<file_data> get_files_in_directory(const std::string& dir_path){
	namespace fs = std::filesystem;
	std::vector<file_data> files;
	fs::path directory(dir_path);
	try{
		for(const auto& entry : fs::directory_iterator(directory)){
			file_data file(
				entry.path(), entry.path().filename(), entry.file_size()
			);
			files.push_back(file);
		}
	} catch(const fs::filesystem_error& ex){
		std::cerr << ex.what() << std::endl;
	}
	return files;
}
} //namespace utils
	
