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
			 if (Pls().Play(true))  plsPlId = plsId;  break;
	key(Z):	 if (Pls().Play(false))  plsPlId = plsId;  break;
	key(X):	 audio->Pause();  break;
	key(C):	 audio->Stop();  break;
	
	key(A):	 Pls().Next(-1);  break;  // |<
	key(S):	 Pls().Next(1);  break;   // >|

	
	key(Q): key(Left):  // seek << >>
		if (alt)  audio->SetVol(1, shift, ctrl);
			else  audio->SetPos(1, shift, ctrl);  break;
	key(W):	key(Right):
		if (alt)  audio->SetVol(0, shift, ctrl);
			else  audio->SetPos(0, shift, ctrl);  break;

	key(E):	 audio->SetVol(0, shift, ctrl);  break;  // volume ^ v
	key(D):	 audio->SetVol(1, shift, ctrl);  break;
	
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
	key(T):	 audio->bRepTrk = !audio->bRepTrk;  Osd("Repeat track: " + b2on(audio->bRepTrk));  break;
	key(R):  audio->bRepPls = !audio->bRepPls;  Osd("Repeat playlist: " + b2on(audio->bRepPls));  break;
	key(P):  set.view.eVis = set.view.eVis == viFFT ? viNone : viFFT;  UpdDim();  break;

	//  debug
	key(I):  bFps = !bFps;  Osd("Fps: " + b2on(bFps));  break;
	key(U):  bDebug = !bDebug;  Osd("Debug: " + b2on(bDebug));  break;
	key(M):	 ++iTimeTest;  if (iTimeTest > 2)  iTimeTest = 0;
			 Osd("Time test: " + !iTimeTest ? "off" : i2s(iTimeTest));  Redraw();  break;
	
	#define OsdStats  Osd("Stats  Full: " + b2on(bFullStats) + "  All: " + b2on(bAllStats))
	key(L):  bAllStats = !bAllStats;  OsdStats;  Redraw();  break;
	key(K):  bFullStats = !bFullStats;  OsdStats;  Redraw();  break;
	//  find
	key(F):  if (alt || ctrl)  WndOpen(WO_PlsFind);
		else {  bFind = !bFind;  Redraw();  }  break;
	
	
	//  playlist
	key(F4):  Pls().Save();  SaveState();  set.Save();  Osd("Saved all.");  break;
	key(F5):  Pls().Load();  Osd("Playlist reloaded.");  break;
	key(Insert):  OpenDirFile(shift);  Osd("Inserted.");
		break;
	key(Delete):
		if (ctrl)  {  Pls().Clear();  Osd("Pls cleared.");  }
		/*else if (shift)
			Pls().DeleteCur();/**/
		break;
	
	//  view modes
	key(Num2):  set.NextDirView(shift ? -1 : 1);  Osd("Dir View: " + string(sDirView[set.eDirView]));  Redraw();  break;
	key(Num3):  set.bFileInfo = !set.bFileInfo;  Osd("FileInfo: " + b2on(set.bFileInfo));  break;
	
	//  gui
	key(F1):  WndOpen(shift ? WO_Main : ctrl ? WO_AppAudio : alt ? WO_PlsFilter : WO_PlsFind);  break;
	key(F2):  WndOpen(ctrl ? WO_AppVis : alt ? WO_AppTabs : WO_PlsTab);  break;
	key(F3):  WndOpen(ctrl ? WO_AppAbout : alt ? WO_AppTest : WO_AppShow);  break;
	// todo: all in popup, or tabs, rmb areas^

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
