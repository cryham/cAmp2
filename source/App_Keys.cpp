#include "App.h"
#include "Audio.h"
using namespace std;  using namespace sf;


bool App::KeyDown(Event::KeyEvent k)
{
	switch (k.code)
	{
	///  player
	case Keyboard::Enter:  // |> || []
	case Keyboard::Z:	pls->Play(true);  break;
	case Keyboard::X:	audio->Pause();  break;
	case Keyboard::C:	audio->Stop();  break;
	
	case Keyboard::A:	pls->Next(-1);  break;  // |<
	case Keyboard::S:	pls->Next(1);  break;   // >|

	case Keyboard::Q:  // seek << >>
	case Keyboard::Left:
		if (k.alt)  audio->chVol(1, shift, ctrl);
			  else  audio->chPos(1, shift, ctrl);  break;
	case Keyboard::W:
	case Keyboard::Right:
		if (k.alt)  audio->chVol(0, shift, ctrl);
			  else  audio->chPos(0, shift, ctrl);  break;

	case Keyboard::E:	audio->chVol(0, shift, ctrl);  break;  // vol ^ v
	case Keyboard::D:	audio->chVol(1, shift, ctrl);  break;
		
	///  playlist  move  cursor, offset
	//  none- up/dn 1   ctrl- x8   // alt- move1
	case Keyboard::Up:		/*if (alt)  pls->Move1(shift? -2: ctrl? 2: -1,0);  else*/  pls->Up(ctrl ? 8:1);  break;
	case Keyboard::Down:	/*if (alt)  pls->Move1(shift? -2: ctrl? 2:  1,0);  else*/  pls->Dn(ctrl ? 8:1);  break;

	//  none- page/4   ctrl- page   alt- offset 1   ctrl-alt- ofs x8
	case Keyboard::PageUp:		if (alt)  pls->PgOfsUp(ctrl ? 8:1);  else  pls->PgUp(ctrl ? yL_pl : yL_pl/4);  break;
	case Keyboard::PageDown:	if (alt)  pls->PgOfsDn(ctrl ? 8:1);  else  pls->PgDn(ctrl ? yL_pl : yL_pl/4);  break;
	
	//  none- move by dir   ctrl- view   alt- list
	case Keyboard::Home:	pls->Home(alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;
	case Keyboard::End:		pls->End( alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;

	///  toggle
	case Keyboard::T:	audio->bRepTrk = !audio->bRepTrk;  break;
	case Keyboard::R:	audio->bRepPls = !audio->bRepPls;  break;
	case Keyboard::P:	set.view.eVis = set.view.eVis == viFFT ? viNone : viFFT;  UpdDim();  break;

	//  debug
	case Keyboard::I:	bFps = !bFps;  break;
	case Keyboard::U:	bDebug = !bDebug;  break;
	case Keyboard::M:	++iTimeTest;  if (iTimeTest > 2)  iTimeTest = 0;  bDraw = true;  break;
	
	//  playlist
	case Keyboard::F4:	pls->Save();  break;
	default:  break;
	}
	return true;
}

void App::UpdModifiers()
{
	#define key(k)  Keyboard::isKeyPressed(Keyboard::k)
	alt = key(LAlt) || key(RAlt);
	ctrl = key(LControl) || key(RControl);
	shift = key(LShift) || key(RShift);
	#undef key
}
