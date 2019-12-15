#include "App.h"
#include "AudioBass.h"
#include "FileSystem.h"
#include <memory>
#include <fstream>
#include <iostream>
using namespace std;  using namespace sf;


App::App()
{
}

App::~App()
{
}


//  Init
bool App::Init()
{
	FileSystem::Init();

	//  log
	AppLog::Init();
	
	Log(FileSystem::GetInfo());
	
	
	//  set
	if (!set.Load())
		return false;
	
	
	//  pls
	pls = make_unique<Playlist>();
	pls->AddDir("../../../zm", false);
#if 1
	const auto& tracks = pls->GetTracks();
	Log("---- tracks");
	for (const auto& trk: tracks)
		Log(trk.GetName());
#endif

	//  audio
	audio = make_unique<AudioBass>();
	audio->Init();

	return true;
}


//  Destroy
bool App::Destroy()
{
	//  set
	bool ok = set.Save();

	//  audio
	audio->Destroy();
	
	return ok;
}
