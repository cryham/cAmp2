#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//------------------------------------------------------------------
///  Pls Find
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsFind()
{
	bool e;
	static char s[1024]={0};  strcpy(s, sFind.c_str());  WndFocus();
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
			ImGui::Text("Found: See %d Vis %d / Pls %d  All %d",
				iFoundSee, Pls().GetFoundVis(), Pls().GetFound(), iFoundAll);
	}else
		TextG("");
	
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
	i = Pls().filterLow;  s = "Rating Low: " + i2s(i) +" "+ Ratings::GetVis(i);  TextG(s);  WndFocus();
	e = SliderInt("rFl", &i, Ratings::valMin, Ratings::valMax, "");
	if (e) {  Pls().filterLow = i;  Pls().UpdateVis(0, true);  }
	
	i = Pls().filterHigh;  s = "Rating High: " + i2s(i) +" "+ Ratings::GetVis(i);  TextG(s);
	e = SliderInt("rFh", &i, Ratings::valMin, Ratings::valMax, "");
	if (e) {  Pls().filterHigh = i;  Pls().UpdateVis(0, true);  }
	PopItemWidth();

	// todo:  -show empty dirs  `dir2 separator lines
}


//  App Stats
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_PlsStats()
{
	bool e;  string s;
	Sep(10);
	e = Checkbox("All playlists", &bAllStats);  if (e) Redraw();
	e = Checkbox("Full unfiltered", &bFullStats);  if (e) Redraw();
	
	//  write stats
	Sep(10);
	uint di, fi, si, tm;
	
	const Stats& ful = bAllStats ?
					allFull : Pls().statsAll;
	const Stats& st = bAllStats ?
		bFullStats ? allFull : all :
		bFullStats ? Pls().statsAll : Pls().stats;

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
		
		s = " Dirs:  "+i2s(di);    TextG(s);  SameLine(200);  s = f2s(Fdi,2,5)+" %";  TextG(s);
		s = "Files:  "+i2s(fi);    TextG(s);  SameLine(200);  s = f2s(Ffi,2,5)+" %";  TextG(s);
		s = " Size:  "+size2s(si); TextG(s);  SameLine(200);  s = f2s(Fsi,2,5)+" %";  TextG(s);
		s = "Time:  "+time2s(tm);  TextG(s);  SameLine(200);  s = f2s(Ftm,2,5)+" %";  TextG(s);
	}
}
