#include <iostream>

#include "FileSystem.h"
#include "AppSFMLDraw.h"
using namespace std;


int main()
{
    FileSystem::Init();
    cout << FileSystem::GetInfo();

	unique_ptr<AppSFMLDraw> app = make_unique<AppSFMLDraw>();
	bool ok = app->Run();
	
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
