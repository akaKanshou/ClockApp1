#ifndef FILE_DET_H
#define FILE_DET_H

#include <string>
#include <filesystem>

#if defined(_WIN32)
#define NOMINMAX
#  include <windows.h>
#else
#  include <unistd.h>
#endif

std::filesystem::path getCur();

std::string getAsset(std::string assetname);

#endif 
