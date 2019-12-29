#include "App.h"
#include "Audio.h"
using namespace std;  using namespace sf;


bool App::KeyDown(sf::Event::KeyEvent k)
{
	alt = k.alt;  ctrl = k.control;  shift = k.shift;
	switch (k.code)
	{
	case Keyboard::Enter:  // |> || []
	case Keyboard::Z:	Play();  break;
	case Keyboard::X:	audio->Pause();  break;
	case Keyboard::C:	audio->Stop();  break;
	
//	case Keyboard::A:	audio->Prev();  break;
//	case Keyboard::S:	audio->Next();  break;

	//  player
	case Keyboard::Q:  // << >>
	case Keyboard::Left:
		if (k.alt)  audio->chVol(1, shift, ctrl);
			  else  audio->chPos(1, shift, ctrl);  break;
	case Keyboard::W:
	case Keyboard::Right:
		if (k.alt)  audio->chVol(0, shift, ctrl);
			  else  audio->chPos(0, shift, ctrl);  break;

	case Keyboard::E:  // vol ^ v
		audio->chVol(0, shift, ctrl);  break;
	case Keyboard::D:
		audio->chVol(1, shift, ctrl);  break;
		
	//  playlist move  cursor, offset
	//  none- up/dn 1   ctrl- x8  //  alt- move1
	case Keyboard::Up:		/*if (alt)  pls->Move1(shift? -2: ctrl? 2: -1,0);  else*/  pls->Up(ctrl ? 8:1);  break;
	case Keyboard::Down:	/*if (alt)  pls->Move1(shift? -2: ctrl? 2:  1,0);  else*/  pls->Dn(ctrl ? 8:1);  break;

	//  none- page/4   ctrl- page   alt- offset 1   ctrl-alt- ofs x8
	case Keyboard::PageUp:		if (alt)  pls->PgOfsUp(ctrl ? 8:1);  else  pls->PgUp(ctrl ? yL_pl : yL_pl/4);  break;
	case Keyboard::PageDown:	if (alt)  pls->PgOfsDn(ctrl ? 8:1);  else  pls->PgDn(ctrl ? yL_pl : yL_pl/4);  break;
	
	//  none- move by dir   ctrl- view   alt- list
	case Keyboard::Home:	pls->Home(alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;
	case Keyboard::End:		pls->End( alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;

	//  toggle
	case Keyboard::T:	audio->bRep1 = !audio->bRep1;  break;
	//  debug
	case Keyboard::I:	bFps = !bFps;  break;
	case Keyboard::U:   bDebug = !bDebug;  break;
	case Keyboard::M:   ++iTimeTest;  if (iTimeTest > 2)  iTimeTest = 0;  break;
		
	default:  break;
	}
	return true;
}

bool App::KeyUp(sf::Event::KeyEvent k)
{
	alt = k.alt;  ctrl = k.control;  shift = k.shift;
	return true;
}


//  move to pls ..
void App::Play()
{
	if (pls->IsEmpty())
		return;
	//if (pls->Check())
	
	Track& t = pls->GetTracks()[pls->cur];
	audio->Play(t);
	
	pls->play = pls->cur;
}
