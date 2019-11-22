#include <iostream>

#include "FileSystem.h"
#include "Playlist.h"
#include "AppSFMLDraw.h"
using namespace std;


int main()
{
    FileSystem::Init();
    cout << FileSystem::GetInfo();

	//  laod Settings first
	//------------------
//	App* app = new App();
//	app->set.Load();
//	Settings& set = app->set;
	
    //LogFile::Create(FileSystem::Config());

//	unique_ptr<App> app = make_unique<App>();
//	app->Run();

	unique_ptr<AppSFMLDraw> app = make_unique<AppSFMLDraw>();
	app->Run();
	
    //LoadSettings(FileSystem::Settings());

    //LoadTexture(FileSystem::Data());

	/*unique_ptr<Playlist> p = make_unique<Playlist>();
    p->AddDir("../../../zm");

    const auto& tracks = p->GetTracks();
    for (const auto& trk: tracks)
        cout << trk.GetName() << endl;*/
	
    return 0;
}
