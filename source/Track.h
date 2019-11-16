#pragma once

#include <string>
#include <filesystem>
namespace fs = std::filesystem;


class Track
{
protected:

    //  shown in gui, from file name
    std::string name;

    //  path to file on disk, saved in playlist
    fs::path path;

public:
    //Track();
    Track(fs::path file);

    void SetNameFromPath();

    std::string GetName() const
    {   return name;    }

};
