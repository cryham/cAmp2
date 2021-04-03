#include "../libs/imgui.h"
#include "../libs/imgui-SFML.h"
#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
using namespace sf;  using namespace std;  using namespace ImGui;
#define TextG  ImGui::Text


//  Pls Find
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsFind()
{
	//  tabs
	//Sep(5);
	//TabLabels(Tab_ALL, tabNames, tab, true);
	//Sep(5);  Line(cl0);  Sep(5);
	
	//Sep(10);  Line(cl0);  Sep(10);
	static bool findCase=false, findWhole=false, findUnfiltered=true, findAllPls=true, findFullPath=false;
	static char sFind[1024]={0};

	/*if (findFocus)  // after alt-F
	{	findFocus = false;  SetKeyboardFocusHere();
	}*/
	bool e;
	PushItemWidth(140);  e = InputText("find", sFind, sizeof(sFind));  PopItemWidth();
	//SameLine();  //if (e)  DoFind();
	e = Button("X Hide");  //if (e) {  sFind[0]=0;  DoFind();  }
	SameLine(110);  e = Button("< Prev");  //if (e)  NextFind(-dFind);
	SameLine(180);  e = Button("Next >");  //if (e)  NextFind( dFind);

	//if (sFind[0]) {  SameLine(140);  Text("Found: %d  visible: %d", iFoundAll, iFound);  }
	e = false;
	TextG("");
	e |= Checkbox("Case sens", &findCase);
	e |= Checkbox("Whole words", &findWhole);
	e |= Checkbox("Full path", &findFullPath);
	TextG("");
	// parent parent2  full path  only fname?
	e |= Checkbox("Full, unfiltered", &findUnfiltered);
	e |= Checkbox("All playlists", &findAllPls);
	//if (e)  DoFind();
}

//  Pls Filter
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsFilter()
{
	// todo:  lower  upper  -show empty dirs  `dir2 separator lines
}

//  Tabs
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsTab()
{
	bool e;  string s;  int i;
	static char t[1024]={0};
	strcpy(t, Pls().name.c_str());  TextG("Rename tab:");
	e = InputText("Tbn", t, sizeof(t));  if (e) Pls().name = t;

	TextG("");
	i = Pls().bookm;  s = "Bookmark: " + i2s(i);  TextG(s.c_str());
	PushItemWidth(140);  e = SliderInt("Tbk", &i, 0, 6, "");  PopItemWidth();
	if (e) {  Pls().bookm = i;  Redraw();  }
	
	// todo: h,s,v sldr color

	TextG("");
	TextG("Tabs counts:");
	i = set.view.xNpt;  s = "columns: " + i2s(i);  TextG(s.c_str());
	PushItemWidth(140);  e = SliderInt("Tbx", &i, 0, 30, "");  PopItemWidth();
	if (e) {  set.view.xNpt = i;  Redraw();  }
	
	i = set.view.yNpt;  s = "rows: " + i2s(i);  TextG(s.c_str());
	PushItemWidth(140);  e = SliderInt("Tby", &i, 0, 30, "");  PopItemWidth();
	if (e) {  set.view.yNpt = i;  Redraw();  }
}

//  App Show
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppShow()
{
	bool e;  string s;
	Checkbox("File Info (from cursor)", &set.bFileInfo);
	
	TextG("");
	int i = set.eDirView;
	s = string("Dir View: ") + sDirView[i];
	TextG(s.c_str());
	PushItemWidth(140);  e = SliderInt("dirv", &i, 0, DV_All-1, "");  PopItemWidth();
	if (e) {  set.eDirView = (EDirView)i;  Redraw();  }
	
	TextG("");
	TextG("Playlists Statistics:");
	e = Checkbox("All playlists", &bAllStats);  if (e) Redraw();
	e = Checkbox("Full unfiltered", &bFullStats);  if (e) Redraw();
	
	//  write stats
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
	s = " Time: "+time2s(tm);  TextG(s.c_str());
}

//  Audio
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppAudio()
{
	// todo:  freq  snd device  volume  balance-
	bool e;  string s;
	TextG("Repeat");
	Checkbox("Track", &audio->bRepTrk);
	Checkbox("Playlist", &audio->bRepPls);
}

//  Vis
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppVis()
{
	// todo:  vis type  size  sleep ms
	//set.view.eVis == viFFT ? viNone : viFFT;  UpdDim();
}

//  Test
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppTest()
{
	bool e;  string s;
	TextG("Test");
	Checkbox("Fps, frames per second drawn", &bFps);
	Checkbox("Debug text", &bDebug);
	
	s = string("Time colors: ") + sTimesTest[iTimeTest];  TextG(s.c_str());
	PushItemWidth(140);  e = SliderInt("test", &iTimeTest, 0, 2, "");  PopItemWidth();  if (e) Redraw();
}
