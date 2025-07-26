#ifndef FILE_DET_H
#define FILE_DET_H

#include <string>
#include <filesystem>

#if defined(_WIN32)
#	include<Windows.h>
#else
#	include<unistd.h>
#endif

namespace FileDet {
	enum ASSET_TYPE {
		SHADER = 0,
		FONT = 1,
		IMAGE = 2,

		ERROR_TYPE
	};

	std::filesystem::path exeDir();

	std::filesystem::path getRoot();

	std::string getPath(FileDet::ASSET_TYPE type, std::string name);
};

#endif 
