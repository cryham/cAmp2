#include "../libs/imgui.h"
#include "../libs/imgui-SFML.h"
#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "Settings.h"
using namespace sf;  using namespace std;  using namespace ImGui;
#define TextG  ImGui::Text


//------------------------------------------------------------------
///  Pls Find
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsFind()
{
	/*if (findFocus)  // after alt-F
	{	findFocus = false;  SetKeyboardFocusHere();  }*/
	bool e;
	static char s[1024]={0};
	strcpy(s, sFind.c_str());
	PushItemWidth(140);  e = InputText("find", s, sizeof(s));  PopItemWidth();
	if (e)  {  sFind = s;  Find();  }
	SameLine(200);
	e = Button("X Clear");  if (e) {  sFind.clear();  Find();  }
	e = Checkbox("Show", &bFind);  if (e)  Redraw();
	SameLine(120);  e = Button("< Prev");  //if (e)  NextFind(-dFind);
	SameLine(200);  e = Button("Next >");  //if (e)  NextFind( dFind);
	// todo: goto next,prev
	// todo: trk time range

	Sep(10);
	if (!sFind.empty())
	{	if (!iFoundAll)
			TextG("Not found.");
		else
			TextG("Found:  %d visible of %d  All %d", iFoundVis, Pls().iFound, iFoundAll);
	}else
		Sep(10);
	Sep(10);
	auto& f = set.find;
	e = false;
	e |= Checkbox("Case sensitive", &f.bCaseSens);
	e |= Checkbox("In full path", &f.bFullPath);  // only path, fname?
	e |= Checkbox("Full playlist, unfiltered", &f.bUnfiltered);
	e |= Checkbox("All playlists", &f.bAllPls);
	if (e)  Find();
}

//  Pls Filter
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsFilter()
{
	bool e;  string s;  int i;

	PushItemWidth(300);
	i = Pls().filterLow;  s = "Rating Low: " + i2s(i) +" "+ GetRateStr(i);  TextG(s.c_str());
	e = SliderInt("rFl", &i, cRateMin, cRateMax, "");
	if (e) {  Pls().filterLow = i;  Redraw();  }
	
	i = Pls().filterHigh;  s = "Rating High: " + i2s(i) +" "+ GetRateStr(i);  TextG(s.c_str());
	e = SliderInt("rFh", &i, cRateMin, cRateMax, "");
	if (e) {  Pls().filterHigh = i;  Redraw();  }
	PopItemWidth();

	// todo:  -show empty dirs  `dir2 separator lines
}


//------------------------------------------------------------------
///  Pls Tab
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsTab()
{
	bool e;  string s;  int i;
	static char t[1024]={0};

	strcpy(t, Pls().name.c_str());  TextG("Name:");
	PushItemWidth(300);  
	e = InputText("Tbn", t, sizeof(t));  if (e) Pls().name = t;

	Sep(10);
	i = Pls().bookm;  s = "Bookmark: " + i2s(i);  TextG(s.c_str());
	e = SliderInt("Tbk", &i, 0, 6, "");
	if (e) {  Pls().bookm = i;  Redraw();  }
	
	Sep(10);  // h,s,v
	float hsv[3] = {Pls().hue, Pls().sat, Pls().val};
	e = ColorPicker3("Color", hsv, ImGuiColorEditFlags_PickerHueBar|ImGuiColorEditFlags_InputHSV);
	if (e) {  Pls().hue = hsv[0];  Pls().sat = hsv[1];  Pls().val = hsv[2];  Pls().UpdateColor();  }
	PopItemWidth();
}

//  App Tabs
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppTabs()
{
	bool e;  string s;  int i;
	PushItemWidth(300);  
	Sep(10);
	i = set.view.xNpt;  s = "Columns: " + i2s(i);  TextG(s.c_str());
	e = SliderInt("Tbx", &i, 1, 30, "");
	if (e) {  set.view.xNpt = i;  UpdDim();  }
	
	i = set.view.yNpt;  s = "Rows: " + i2s(i);  TextG(s.c_str());
	e = SliderInt("Tby", &i, 1, 30, "");
	if (e) {  set.view.yNpt = i;  UpdDim();  }

	Sep(10);
	auto fp = [](int i){  return f2s(100.f*i/16.f,0,3) + " %%";  };
	i = set.dimTabBck;  s = "Dim background: " + fp(i);  TextG(s.c_str());
	e = SliderInt("Tdb", &i, 1, 16, "");
	if (e) {  set.dimTabBck = i;  }

	i = set.dimTabTxt;  s = "Dim text: " + fp(i);  TextG(s.c_str());
	e = SliderInt("Tdt", &i, 1, 16, "");
	if (e) {  set.dimTabTxt = i;  }
	PopItemWidth();
}


//------------------------------------------------------------------
//  App Show
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppShow()
{
	bool e;  string s;
	Checkbox("File Info (from cursor)", &set.bFileInfo);
	
	Sep(10);
	int i = set.eDirView;
	s = string("Dir View: ") + sDirView[i];
	TextG(s.c_str());
	PushItemWidth(140);  e = SliderInt("dirv", &i, 0, DV_All-1, "");  PopItemWidth();
	if (e) {  set.eDirView = (EDirView)i;  Redraw();  }
	
	Line();
	TextG("Playlists Statistics:");
	e = Checkbox("All playlists", &bAllStats);  if (e) Redraw();
	e = Checkbox("Full unfiltered", &bFullStats);  if (e) Redraw();
	
	//  write stats
	Sep(10);
	uint di, fi, si, tm;
	const Stats& st = bAllStats ?
		bFullStats ? allFull : all :
		bFullStats ? Pls().stAll : Pls().stats;
	di = st.GetDirs();
	fi = st.GetFiles();
	si = st.GetSize() / 1000000;
	tm = st.GetTime();
	s = " Dirs: "+i2s(di);  TextG(s.c_str());
	s = "Files: "+i2s(fi);  TextG(s.c_str());
	s = " Size: "+size2s(si);  TextG(s.c_str());
	s = "Time: "+time2s(tm);  TextG(s.c_str());
}

//  Audio
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppAudio()
{
	// todo:  freq  snd device  volume  balance-
	bool e;  string s;
	
	int i = audio->iVolume;
	s = string("Volume: ") + f2s(i/10.f, 1,4) + " %%";  TextG(s.c_str());
	PushItemWidth(240);  e = SliderInt("vol", &i, 0, 1000, "");  PopItemWidth();
	if (e) {  audio->iVolume = i;  Redraw();  }
	
	Sep(10);
	TextG("Repeat");
	Checkbox("Track", &audio->bRepTrk);  SameLine(150);
	Checkbox("Playlist", &audio->bRepPls);
	
	i = 0;  // play controls meh
	Sep(10);
	e = Button("|<");  i += 80;  SameLine(i);  if (e)  Pls().Next(-1);
	e = Button("|>");  i += 60;  SameLine(i);  if (e)  if (Pls().Play(false))  plsPlId = plsId;
	e = Button("||");  i += 50;  SameLine(i);  if (e)  audio->Pause();
	e = Button("[]");  i += 50;  SameLine(i);  if (e)  audio->Stop();
	e = Button(">|");  i += 50;  SameLine(i);  if (e)  Pls().Next(1);
}

//  Visualization
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppVis()
{
	bool e;  string s;
	int i = set.view.iFFTSize;
	s = string("FFT size: ") + i2s(i);  TextG(s.c_str());
	PushItemWidth(240);  e = SliderInt("fftsi", &i, 0, ViewSet::ciFFTNum-1, "");  PopItemWidth();
	if (e)  set.view.iFFTSize = i;

	float f = set.view.fFFTMul;
	s = string("FFT scale: ") + f2s(f, 2, 5);  TextG(s.c_str());
	PushItemWidth(240);  e = SliderFloat("fftmul", &f, 0, 200.f, "");  PopItemWidth();
	if (e)  set.view.fFFTMul = f;
//	int eVis = viFFT;  // todo: type
//	int iSleep = 0;  // in ms
//	bool bVSync = true;
//	int iVisH = 96;  // draw height
//	float fPrtFq = 100.f;  // spectrogram speed
}


//------------------------------------------------------------------
//  Test
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppTest()
{
	bool e;  string s;
	Checkbox("Fps, frames per second drawn", &bFps);
	Checkbox("Debug text", &bDebug);
	
	s = string("Time colors: ") + sTimesTest[iTimeTest];  TextG(s.c_str());
	PushItemWidth(140);  e = SliderInt("test", &iTimeTest, 0, 2, "");  PopItemWidth();  if (e) Redraw();
}


//  About
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppAbout()
{
	Sep(20);
	TextG("Crystal AMP 2  (cAmp2)");
	Sep(10);
	TextG("by Crystal Hammer");
	Sep(20);
	TextG("Version: 2.0.4");
	//TextG(Settings::ver);
	Sep(40);
	PushItemWidth(400);
	TextG("https://github.com/cryham/cAmp2");
//	auto fl = ImGuiInputTextFlags_ReadOnly;
//	InputText("Url1", url1, sizeof(url1), fl);
	Sep(10);
	TextG("http://cryham.tuxfamily.org/portfolio/2010_camp/");
	PopItemWidth();
}

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
