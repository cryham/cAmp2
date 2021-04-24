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
	
	int i = audio->iVolume;
	s = string("Volume: ") + f2s(i/10.f, 1,4) + " %%";  TextG(s);
	PushItemWidth(300);  e = SliderInt("vol", &i, 0, 1000, "");  PopItemWidth();
	if (e) {  audio->iVolume = i;  audio->SetVolAbs();  Redraw();  }
	
	Sep(10);
	TextG("Repeat");
	Checkbox("Track", &audio->bRepTrk);  SameLine(150);
	Checkbox("Playlist", &audio->bRepPls);
	
	i = 0;  // play controls meh
	Sep(10);
	e = Button("|<");  i += 80;  SameLine(i);  if (e)  Pls().Next(-1);
	e = Button("|>");  i += 60;  SameLine(i);  if (e)  Play(false);
	e = Button("||");  i += 50;  SameLine(i);  if (e)  audio->Pause();
	e = Button("[]");  i += 50;  SameLine(i);  if (e)  audio->Stop();
	e = Button(">|");  i += 50;  SameLine(i);  if (e)  Pls().Next(1);
	// todo: pls dir play next/prev
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


//  About
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppAbout()
{
	Sep(20);
	TextColored(ImVec4(0.5f, 0.97f, 1.0f, 1.f), "Crystal AMP 2  (cAmp2)");
	Sep(10);
	TextColored(ImVec4(0.7f, 0.67f, 1.0f, 1.f), "by Crystal Hammer");
	Sep(20);
	TextColored(ImVec4(0.8f, 0.9f, 1.0f, 1.f), "Version: 2.1.1");
	//TextG(Settings::ver);
	Sep(40);
	PushItemWidth(400);

	TextG("Sources, Issues etc.");
	auto url1 = "https://github.com/cryham/cAmp2";
	if (Button(url1))  OpenBrowserUrl(url1);

	Sep(10);
	TextG("Project description");
	auto url2 = "https://cryham.tuxfamily.org/portfolio/2010_camp/";
	if (Button(url2))  OpenBrowserUrl(url2);
	
	PopItemWidth();
}
