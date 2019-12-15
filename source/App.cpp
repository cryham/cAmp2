#include "App.h"
#include "AudioBass.h"
#include <memory>
using namespace std;  using namespace sf;


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

	audio = make_unique<AudioBass>();
	audio->Init();

	return true;
}

bool App::Destroy()
{
	bool ok = set.Save();
	audio->Destroy();
	return ok;
}


bool App::KeyDown(sf::Event::KeyEvent k)
{
	switch (k.code)
	{
	case Keyboard::Z:	audio->Play();  break;
	case Keyboard::X:	audio->Pause();  break;
	case Keyboard::C:	audio->Stop();  break;
	
//	case Keyboard::A:	audio->Prev();  break;
//	case Keyboard::S:	audio->Next();  break;

	//  player
	case Keyboard::Q:
	case Keyboard::Left:
		if (k.alt)  audio->chVol(1, k.shift, k.control);  else  audio->chPos(1, k.shift, k.control);  break;
	case Keyboard::W:
	case Keyboard::Right:
		if (k.alt)  audio->chVol(0, k.shift, k.control);  else  audio->chPos(0, k.shift, k.control);  break;

	case Keyboard::I:	audio->bFps = !audio->bFps;  break;
		
	default:  break;
	}
	return true;
}

bool App::KeyUp(sf::Event::KeyEvent key)
{
	
	return true;
}
