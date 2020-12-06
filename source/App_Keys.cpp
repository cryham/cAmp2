#include "App.h"
#include "Audio.h"
#include "Util.h"
#include <SFML/Window/Event.hpp>  // input, use own enum?
using namespace std;  using namespace sf;


bool App::KeyDown(Event::KeyEvent k)
{
	switch (k.code)
	{
	#define key(k)  case Keyboard::k
	
	///  player
	key(Enter):  // |> || []
	key(Z):	 if (Pls().Play(true))  plsPlId = plsId;  break;
	key(X):	 audio->Pause();  break;
	key(C):	 audio->Stop();  break;
	
	key(A):	 Pls().Next(-1);  break;  // |<
	key(S):	 Pls().Next(1);  break;   // >|

	
	key(Q): key(Left):  // seek << >>
		if (alt)  audio->chVol(1, shift, ctrl);
			else  audio->chPos(1, shift, ctrl);  break;
	key(W):	key(Right):
		if (alt)  audio->chVol(0, shift, ctrl);
			else  audio->chPos(0, shift, ctrl);  break;

	key(E):	 audio->chVol(0, shift, ctrl);  break;  // vol ^ v
	key(D):	 audio->chVol(1, shift, ctrl);  break;
	
	key(Backspace):
		if (ctrl)  plsId = plsPlId;
		Pls().GotoPlay();  break;
		
	
	///  playlist  move  cursor, offset
	//  none- up/dn 1   ctrl- x8   // alt- move1
	key(Up):	/*if (alt)  Pls().Move1(shift? -2): ctrl? 2): -1,0);  else*/  Pls().Up(ctrl ? 8:1);  break;
	key(Down):	/*if (alt)  Pls().Move1(shift? -2): ctrl? 2):  1,0);  else*/  Pls().Dn(ctrl ? 8:1);  break;

	//  none- page/4   ctrl- page   alt- offset 1   ctrl-alt- ofs x8
	key(PageUp):	if (alt)  Pls().PgOfsUp(ctrl ? 8:1);  else  Pls().PgUp(ctrl ? yL_pl : yL_pl/4);  break;
	key(PageDown):	if (alt)  Pls().PgOfsDn(ctrl ? 8:1);  else  Pls().PgDn(ctrl ? yL_pl : yL_pl/4);  break;
	
	//  none- move by dir   ctrl- view   alt- list
	key(Home):	Pls().Home(alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;
	key(End):	Pls().End( alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;

	
	///  rate file - +
	// none- playing cur  ctrl- cursor
	key(Subtract):  key(Hyphen):  Pls().Rate(!ctrl,-1);  break;
	key(Add):       key(Equal):   Pls().Rate(!ctrl, 1);  break;

	//  rating filter
	key(Divide):    key(LBracket):  Pls().Filter(!ctrl,-1);  break;
	key(Multiply):  key(RBracket):  Pls().Filter(!ctrl, 1);  break;

	//  next/prev` tab   ctrl- dn/up row  shift- ofs row
	key(Tilde):  TabNext(-1,ctrl,shift);  break;
	key(Tab):    TabNext( 1,ctrl,shift);  break;

	key(Backslash):  // bookmark
		Pls().Bookm(alt, ctrl||shift ? -1 : 1);
		Redraw();  break;

	//  New tab
	key(N):  TabNew(alt? -1: ctrl? 2 : shift? -2: 1);  break;
	//  Close tab
	key(F8): TabClose();  break;
	// todo: F2 rename ..
	
	
	///  toggle
	key(T):	 audio->bRepTrk = !audio->bRepTrk;  break;
	key(R):  audio->bRepPls = !audio->bRepPls;  break;
	key(P):  set.view.eVis = set.view.eVis == viFFT ? viNone : viFFT;  UpdDim();  break;

	//  debug
	key(I):  bFps = !bFps;  break;
	key(U):  bDebug = !bDebug;  break;
	key(M):	 ++iTimeTest;  if (iTimeTest > 2)  iTimeTest = 0;  Redraw();  break;
	
	
	//  playlist
	key(F4):  Pls().Save();  break;
	key(Delete):  if (ctrl)  Pls().Clear();  break;
	//  test--
	key(F10):  Pls().AddDir("../../../../m" /*, false*/);  Redraw();  break;
	key(F11):  Pls().AddDir("../../m" /*, false*/);  Redraw();  break;
	
	#undef key
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
