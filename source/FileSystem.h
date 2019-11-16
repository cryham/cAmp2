#pragma once
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;


class FileSystem
{
public:
    static std::vector<fs::path> ListDir(const fs::path start);

};
