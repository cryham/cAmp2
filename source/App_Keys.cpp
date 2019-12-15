#include "App.h"
#include "Audio.h"
using namespace std;  using namespace sf;


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

	case Keyboard::T:	audio->bRep1 = !audio->bRep1;  break;
	case Keyboard::I:	bFps = !bFps;  break;
		
	default:  break;
	}
	return true;
}

bool App::KeyUp(sf::Event::KeyEvent key)
{
	
	return true;
}
