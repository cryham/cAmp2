#include <iostream>

#include "FileSystem.h"
using namespace std;


int main()
{
    const auto& files = FileSystem::ListDir(".");
    for (const auto& name: files)
        cout << name << endl;

    return 0;
}
