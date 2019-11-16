#include <algorithm>
#include <iostream>
#include "FileSystem.h"
using namespace std;


vector<fs::path> FileSystem::ListDir(const fs::path dir)
{
    vector<fs::path> files;
    try
    {
        auto it = fs::recursive_directory_iterator(dir);
        for (const auto& path: it)
            files.push_back(path);

        sort(files.begin(), files.end());
    }
    catch (exception ex)
    {
        cerr << ex.what() << endl;
    }
    return files;
}

