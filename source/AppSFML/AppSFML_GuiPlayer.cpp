#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//  Main
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_Main()
{
	#if 0  // test imgui demos
	static bool a = true;
	ShowDemoWindow(&a);
	return;
	#endif
	PushItemWidth(300);
	for (int i=0; i < WO_Main; ++i)
	{
		if (i==WO_Tab || i==WO_PlsFind || i==WO_AppAbout)
			Line();
		bool e = Button(wndConst[i].FullTitle().c_str());
		if (e)  wndOpen = (EWndOpt)i;  // schedule to open
		if (i%3)  SameLine();
	}
	PopItemWidth();
}


//  Audio
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppAudio()
{
	// todo:  freq  snd device  volume  balance-
	bool e;  string s;

	Sep(0);  int i = 0;  // play controls
	e = Button("|<");  i += 80;  SameLine(i);  if (e)  Pls().Next(-1);
	e = Button("|>");  i += 60;  SameLine(i);  if (e)  Play(false);
	e = Button("||");  i += 50;  SameLine(i);  if (e)  audio->Pause();
	e = Button("[]");  i += 50;  SameLine(i);  if (e)  audio->Stop();
	e = Button(">|");  i += 50;  SameLine(i);  if (e)  Pls().Next(1);

	Sep(50);
	i = audio->iVolume;
	s = string("Volume: ") + f2s(i/10.f, 1,4) + " %";  TextG(s);
	PushItemWidth(300);  e = SliderInt("vol", &i, 0, 1000, "");  PopItemWidth();
	if (e) {  audio->iVolume = i;  audio->SetVolAbs();  Redraw();  }
	
	Sep(10);
	TextG("Repeat");
	Checkbox("Track", &audio->bRepTrk);  SameLine(150);
	Checkbox("Playlist", &audio->bRepPls);
	
	Sep(10);
	TextG("Play Direction:");
	auto& b = audio->bDirNext;
	if (RadioButton("Down", b))  b = true;  SameLine(150);
	if (RadioButton("Up", !b))  b = false;
}


//------------------------------------------------------------------
//  Test
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppTest()
{
	bool e;  string s;
	Checkbox("Fps, frames per second drawn", &bFps);
	Checkbox("Debug text", &bDebug);
	
	Sep(10);
	if (SliderI(eTimeTest, 0, TimT_ALL-1,
			"Time colors: ", "test", csTimesTest[eTimeTest]))
		Redraw();
}
