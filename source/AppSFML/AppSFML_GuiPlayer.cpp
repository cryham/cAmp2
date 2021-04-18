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
	PushItemWidth(300);
	for (int i=0; i < WO_Main; ++i)
	{
		if (i==WO_PlsTab || i==WO_AppAudio || i==WO_AppAbout)
			Line();
		bool e = Button(wndConst[i].title.c_str());
		if (e)  wndOpen = (EWndOpt)i;  // schedule to open
	}
	PopItemWidth();
}


//------------------------------------------------------------------
//  App View
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppView()
{
	bool e;  string s;
	Checkbox("File Info (from cursor)", &set.bFileInfo);
	
	Sep(10);
	int i = set.eDirView;
	if (SliderI(i, 0, DirV_All-1, "Dir View: ", "dirv", csDirView[i]))
	{	set.eDirView = (EDirView)i;  Redraw();  }

	Sep(10);
	if (Checkbox("Slider draw all ratings", &set.view.sldr.drawRate))  Redraw();

	Sep(5);
	if (SliderI(set.view.sldr.width, 2, 60, "Slider width: ", "sldW"))  UpdDim();

	Sep(10);  Line();  e = false;
	TextG("Font sizes:");	xSlider = 200;
	for (int i=0; i < Fnt_All; ++i)
	{
		auto& f = set.view.fnt[i];
		TextG(csFonts[i]);
		PushItemWidth(50);
		s = " + "+i2s(i);  SameLine(200);  if (Button(s.c_str())) {  e = true;  ++f.size;  ++f.height;  }
		s = " - "+i2s(i);  SameLine(280);  if (Button(s.c_str())) {  e = true;  --f.size;  --f.height;  }
		PopItemWidth();
		e |= SliderI(f.size, 1, 40, "  Size: ", "fntS" + i2s(i));
		if (i == Fnt_Tabs || i == Fnt_Tracks)  // rest unused
			e |= SliderI(f.lineSpacing, -5, 20, "  Spacing: ", "fntH" + i2s(i));
	}
	if (e)
	{	LoadFonts();  UpdDim();  }
	xSlider = 0;
}


//  App Stats
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppStats()
{
	bool e;  string s;
	Sep(10);
	e = Checkbox("All playlists", &bAllStats);  if (e) Redraw();
	e = Checkbox("Full unfiltered", &bFullStats);  if (e) Redraw();
	
	//  write stats
	Sep(10);
	uint di, fi, si, tm;
	const Stats& ful = bAllStats ?
					allFull : Pls().stAll;
	const Stats& st = bAllStats ?
		bFullStats ? allFull : all :
		bFullStats ? Pls().stAll : Pls().stats;
	di = st.GetDirs();
	fi = st.GetFiles();
	si = st.GetSize() / 1000000;
	tm = st.GetTime();
	if (bFullStats || ful.GetFiles() == 0)
	{
		s = " Dirs:  "+i2s(di);    TextG(s);
		s = "Files:  "+i2s(fi);    TextG(s);
		s = " Size:  "+size2s(si); TextG(s);
		s = "Time:  "+time2s(tm);  TextG(s);
	}else
	{	float Fdi, Ffi, Fsi, Ftm;  // %
		Fdi = 100.f * di / ful.GetDirs();
		Ffi = 100.f * fi / ful.GetFiles();
		Fsi = 100.f * si /(ful.GetSize() / 1000000);
		Ftm = 100.f * tm / ful.GetTime();
		s = " Dirs:  "+i2s(di);    TextG(s);  SameLine(200);  s = f2s(Fdi,2,5)+" %%";  TextG(s);
		s = "Files:  "+i2s(fi);    TextG(s);  SameLine(200);  s = f2s(Ffi,2,5)+" %%";  TextG(s);
		s = " Size:  "+size2s(si); TextG(s);  SameLine(200);  s = f2s(Fsi,2,5)+" %%";  TextG(s);
		s = "Time:  "+time2s(tm);  TextG(s);  SameLine(200);  s = f2s(Ftm,2,5)+" %%";  TextG(s);
	}
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
	if (SliderI(eTimeTest, 0, TimT_All-1,
			"Time colors: ", "test", csTimesTest[eTimeTest]))
		Redraw();
}


//  Abou
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppAbout()
{
	Sep(20);
	TextG("Crystal AMP 2  (cAmp2)");
	Sep(10);
	TextG("by Crystal Hammer");
	Sep(20);
	TextG("Version: 2.0.8");
	//TextG(Settings::ver);
	Sep(40);
	PushItemWidth(400);
	// todo: open url? or copy..
	TextG("Sources, Issues etc.");
	TextG("https://github.com/cryham/cAmp2");
//	auto fl = ImGuiInputTextFlags_ReadOnly;
//	InputText("Url1", url1, sizeof(url1), fl);
	Sep(10);
	TextG("Project description");
	TextG("http://cryham.tuxfamily.org/portfolio/2010_camp/");
	PopItemWidth();
}
