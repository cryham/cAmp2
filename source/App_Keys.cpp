#include "App.h"
#include "Audio.h"
using namespace std;  using namespace sf;


bool App::KeyDown(Event::KeyEvent k)
{
	switch (k.code)
	{
	#define key(k)  case Keyboard::k
	
	///  player
	key(Enter):  // |> || []
	key(Z):	 pls->Play(true);  break;
	key(X):	 audio->Pause();  break;
	key(C):	 audio->Stop();  break;
	
	key(A):	 pls->Next(-1);  break;  // |<
	key(S):	 pls->Next(1);  break;   // >|

	
	key(Q): key(Left):  // seek << >>
		if (alt)  audio->chVol(1, shift, ctrl);
			else  audio->chPos(1, shift, ctrl);  break;
	key(W):	key(Right):
		if (alt)  audio->chVol(0, shift, ctrl);
			else  audio->chPos(0, shift, ctrl);  break;

	key(E):	 audio->chVol(0, shift, ctrl);  break;  // vol ^ v
	key(D):	 audio->chVol(1, shift, ctrl);  break;
		
	
	///  playlist  move  cursor, offset
	//  none- up/dn 1   ctrl- x8   // alt- move1
	key(Up):	/*if (alt)  pls->Move1(shift? -2): ctrl? 2): -1,0);  else*/  pls->Up(ctrl ? 8:1);  break;
	key(Down):	/*if (alt)  pls->Move1(shift? -2): ctrl? 2):  1,0);  else*/  pls->Dn(ctrl ? 8:1);  break;

	//  none- page/4   ctrl- page   alt- offset 1   ctrl-alt- ofs x8
	key(PageUp):	if (alt)  pls->PgOfsUp(ctrl ? 8:1);  else  pls->PgUp(ctrl ? yL_pl : yL_pl/4);  break;
	key(PageDown):	if (alt)  pls->PgOfsDn(ctrl ? 8:1);  else  pls->PgDn(ctrl ? yL_pl : yL_pl/4);  break;
	
	//  none- move by dir   ctrl- view   alt- list
	key(Home):	pls->Home(alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;
	key(End):	pls->End( alt ? 2: ctrl ? 1: shift ? -1 : 0);  break;

	
	///  toggle
	key(T):	 audio->bRepTrk = !audio->bRepTrk;  break;
	key(R):  audio->bRepPls = !audio->bRepPls;  break;
	key(P):  set.view.eVis = set.view.eVis == viFFT ? viNone : viFFT;  UpdDim();  break;

	//  debug
	key(I):  bFps = !bFps;  break;
	key(U):  bDebug = !bDebug;  break;
	key(M):	 ++iTimeTest;  if (iTimeTest > 2)  iTimeTest = 0;  bDraw = true;  break;
	
	
	//  playlist
	key(F4):	pls->Save();  break;
	key(Delete):  if (ctrl)  pls->Clear();  break;
	//  test--
	key(F1):  pls->AddDir("../../../../m/spm");  break;
	key(F2):  pls->AddDir("../../../../m/ssm/Music");  break;
	key(F3):  pls->AddDir("../../../zm"/*, false*/);  break;
	
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
