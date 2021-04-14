#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//------------------------------------------------------------------
//  App Show
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppViewStats()
{
	bool e;  string s;
	Checkbox("File Info (from cursor)", &set.bFileInfo);
	
	Sep(10);
	int i = set.eDirView;
	s = string("Dir View: ") + sDirView[i];
	TextG(s);
	PushItemWidth(140);  e = SliderInt("dirv", &i, 0, DirV_All-1, "");  PopItemWidth();
	if (e) {  set.eDirView = (EDirView)i;  Redraw();  }
	
	Sep(5);  Line();
	TextG("Playlists Statistics:");
	Sep(5);
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
	PushItemWidth(240);  e = SliderInt("vol", &i, 0, 1000, "");  PopItemWidth();
	if (e) {  audio->iVolume = i;  Redraw();  }
	
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


void AppSFMLDraw::SliderF(float& fl, float a, float b, const char* text, const char* uid)
{
	float f = fl;
	string s = string(text) + f2s(f, 3, 5);  TextG(s);
	bool e = SliderFloat(text, &f, a, b, "");
	if (e)  fl = f;
}

//------------------------------------------------------------------
//  Visualization
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppVis()
{
	bool e;  string s;  float f;  int i;
	auto& v = set.view.vis;
	PushItemWidth(750);

	i = v.eType;
	s = string("Type: ") + SVisType[i];  TextG(s);
	e = SliderInt("visT", &i, 0, VisT_ALL-1, "");
	if (e) {  v.eType = i;  UpdDim();  }
	
	f = v.yH;  // / set.view.wnd.ySize;  %
	s = string("Height: ") + f2s(f, 0, 4);  TextG(s);
	e = SliderFloat("visH", &f, 0, set.view.wnd.ySize, "");
	if (e) {  v.yH = f;  UpdDim();  }

	//	int iSleep = 0;  // in ms
	//	bool bVSync = true;
	
	Sep(10);
	switch (v.eType)
	{
	case VisT_FFT:
	{	i = v.fft.iSize;
		s = string("FFT size: ") + i2s(i);  TextG(s);
		e = SliderInt("fftsi", &i, 0, ViewSet::FFTSizes-1, "");
		if (e)  v.fft.iSize = i;
	
		f = v.fft.fMul;
		s = string("FFT scale: ") + f2s(f, 2, 5);  TextG(s);
		e = SliderFloat("fftmul", &f, 0, 200.f, "");
		if (e)  v.fft.fMul = f;

		i = colors.curFFT;
		s = "Theme: " + i2s(i)+ "  " + colors.CurFFT().name;  TextG(s);
		e = SliderInt("FTh", &i, 0, colors.vVisual.size()-1, "");
		if (e) {  colors.curFFT = i;
			v.fft.clr = colors.CurFFT();  }
		
		Sep(10);
		auto& c = v.fft.clr;
		SliderF(c.add.h, 0.f, 1.f, "Hue value: ", "FHa");
		SliderF(c.mul.h,-2.f, 2.f, "Hue multi: ", "FHm");
		SliderF(c.pow.h, 0.f, 4.f, "Hue power: ", "FHp");
		Sep(5);
		SliderF(c.add.s,-1.f, 2.f, "Saturation value: ", "FSa");
		SliderF(c.mul.s,-3.f, 3.f, "Saturation multi: ", "FSm");
		SliderF(c.pow.s, 0.f, 4.f, "Saturation power: ", "FSp");
		Sep(5);
		SliderF(c.add.v,-1.f, 2.f, "Brightness value: ", "FVa");
		SliderF(c.mul.v,-3.f, 3.f, "Brightness multi: ", "FVm");
		SliderF(c.pow.v, 0.f, 4.f, "Brightness power: ", "FVp");
	}	break;

	case VisT_Osc:
		//  clr ..
		break;

	case VisT_Spect:
	{	i = v.spect.iSize;
		s = string("FFT size: ") + i2s(i);  TextG(s);
		e = SliderInt("fftsi", &i, 0, ViewSet::FFTSizes-1, "");
		if (e)  v.spect.iSize = i;
	
		f = v.spect.fMul;
		s = string("FFT scale: ") + f2s(f, 2, 5);  TextG(s);
		e = SliderFloat("fftmul", &f, 0, 200.f, "");
		if (e)  v.spect.fMul = f;

		i = colors.curSpect;
		s = "Theme: " + i2s(i)+ "  " + colors.CurSpect().name;  TextG(s);
		e = SliderInt("STh", &i, 0, colors.vVisual.size()-1, "");
		if (e) {  colors.curSpect = i;
			v.spect.clr = colors.CurSpect();  }
		
		Sep(10);
		auto& c = v.spect.clr;
		SliderF(c.add.h, 0.f, 1.f, "Hue value: ", "SHa");
		SliderF(c.mul.h,-2.f, 2.f, "Hue multi: ", "SHm");
		SliderF(c.pow.h, 0.f, 4.f, "Hue power: ", "SHp");
		Sep(5);
		SliderF(c.add.s,-1.f, 2.f, "Saturation value: ", "SSa");
		SliderF(c.mul.s,-3.f, 3.f, "Saturation multi: ", "SSm");
		SliderF(c.pow.s, 0.f, 4.f, "Saturation power: ", "SSp");
		Sep(5);
		SliderF(c.add.v,-1.f, 2.f, "Brightness value: ", "SVa");
		SliderF(c.mul.v,-3.f, 3.f, "Brightness multi: ", "SVm");
		SliderF(c.pow.v, 0.f, 4.f, "Brightness power: ", "SVp");
		
		//	float fPrtFq = 100.f;  // spectrogram speed
	}	break;
	}

	PopItemWidth();
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
	s = string("Time colors: ") + sTimesTest[iTimeTest];  TextG(s);
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
	TextG("Version: 2.0.5");
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
