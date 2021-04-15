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
	s = string("Dir View: ") + csDirView[i];
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
//  Visualization
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppVis()
{
	bool e;  string s;  float f;  int i;
	auto& v = set.view.vis;
	PushItemWidth(750);

	if (SliderI(v.eType, 0, VisT_ALL-1, string("Type: ") + csVisType[v.eType], "visT"))
		UpdDim();
	
	//  / set.view.wnd.ySize;  %
	if (SliderI(v.yH, 0, set.view.wnd.ySize, string("Height: ") + f2s(v.yH, 0, 4), "visH"))
		UpdDim();

	//	int iSleep = 0;  // in ms  // todo:
	//	bool bVSync = true;
	
	auto AddSlidersHSV = [&](VisualColors& c, string s)
	{
		Sep(10);
		SliderF(c.add.h, 0.f, 1.f, "Hue value: ", s+"Ha");
		SliderF(c.mul.h,-2.f, 2.f, "Hue multi: ", s+"Hm");
		SliderF(c.pow.h, 0.f, 4.f, "Hue power: ", s+"Hp");
		Sep(5);
		SliderF(c.add.s,-1.f, 2.f, "Saturation value: ", s+"Sa");
		SliderF(c.mul.s,-3.f, 3.f, "Saturation multi: ", s+"Sm");
		SliderF(c.pow.s, 0.f, 4.f, "Saturation power: ", s+"Sp");
		Sep(5);
		SliderF(c.add.v,-1.f, 2.f, "Brightness value: ", s+"Va");
		SliderF(c.mul.v,-3.f, 3.f, "Brightness multi: ", s+"Vm");
		SliderF(c.pow.v, 0.f, 4.f, "Brightness power: ", s+"Vp");
	};
	
	const int clrs = colors.vVisual.size()-1;
	Sep(10);
	switch (v.eType)
	{
	case VisT_FFT:
	{
		SliderI(v.fft.iSize, 0, ViewSet::FFTSizes-1,
				"FFT size: " + i2s(v.fft.iSize), "fft-siz");
	
		SliderF(v.fft.fMul, 0.f, 200.f,
				"FFT scale: ", "fft-mul");

		if (SliderI(colors.curFFT, 0, clrs,
				"Theme: " + i2s(i)+ "  " + colors.CurFFT().name, "fft-thm"))
			v.fft.clr = colors.CurFFT();
		
		AddSlidersHSV(v.fft.clr, "fft_");
	}	break;

	case VisT_Osc:
	{
		if (SliderI(colors.curOsc, 0, clrs,
				"Theme: " + i2s(i)+ "  " + colors.CurOsc().name, "osc-thm"))
			v.osc.clr = colors.CurOsc();
		
		AddSlidersHSV(v.osc.clr, "osc_");
	}	break;

	case VisT_Spect:
	{
		SliderI(v.spect.iSize, 0, ViewSet::FFTSizes-1,
				"FFT size: " + i2s(v.spect.iSize), "spc-siz");
	
		SliderF(v.spect.fMul, 0.f, 200.f,
				"FFT scale: ", "spc-mul");

		if (SliderI(colors.curSpect, 0, clrs,
				"Theme: " + i2s(colors.curSpect)+ "  " + colors.CurSpect().name, "spc-thm"))
			v.spect.clr = colors.CurSpect();

		i = v.eSpect;
		if (SliderI(i, 0, SpcT_ALL-1,
				"Type: " + string(csSpectType[i]), "spc-typ"))
			v.eSpect = (SpectType)i;
		
		AddSlidersHSV(v.spect.clr, "spc_");
		
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
	s = string("Time colors: ") + csTimesTest[iTimeTest];  TextG(s);
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
