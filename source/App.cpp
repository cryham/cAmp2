#include "App.h"
#include <memory>
using namespace std;


App::App()
{
}
App::~App()
{
}

bool App::Init()
{
	if (!set.Load())
		return false;
	
#if 0
	pls = make_unique<Playlist>();
	pls->AddDir("../../../zm");

	const auto& tracks = pls->GetTracks();
	for (const auto& trk: tracks)
		cout << trk.GetName() << endl;
#endif

	//audio->

	return true;
}

bool App::Destroy()
{
	return set.Save();
}
