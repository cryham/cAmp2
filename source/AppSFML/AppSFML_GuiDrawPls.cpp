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
	static char s[1024]={0};
	strcpy(s, sFind.c_str());  WndFocus();
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
			ImGui::Text("Found:  %d  visible of  %d  All %d", iFoundVis, Pls().GetFound(), iFoundAll);
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
	if (e) {  Pls().filterLow = i;  Pls().UpdateVis();  }
	
	i = Pls().filterHigh;  s = "Rating High: " + i2s(i) +" "+ Ratings::GetVis(i);  TextG(s);
	e = SliderInt("rFh", &i, Ratings::valMin, Ratings::valMax, "");
	if (e) {  Pls().filterHigh = i;  Pls().UpdateVis();  }
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
	PushItemWidth(300);  WndFocus();
	e = InputText("Tbn", t, sizeof(t));  if (e) Pls().name = t;

	Sep(10);
	i = Pls().bookm;  s = "Bookmark: " + i2s(i);  TextG(s);
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
	auto& t = set.view.tabs;

	PushItemWidth(300);  
	i = t.xCols;  s = "Columns: " + i2s(i);  TextG(s);  WndFocus();
	e = SliderInt("Tbx", &i, 1, 30, "");
	if (e) {  t.xCols = i;  UpdDim();  }
	
	i = t.yRows;  s = "Rows: " + i2s(i);  TextG(s);
	e = SliderInt("Tby", &i, 1, 30, "");
	if (e) {  t.yRows = i;  UpdDim();  }

	i = t.ofs;  s = "offset: " + i2s(i);  TextG(s);
	e = SliderInt("Tbo", &i, 0, vPls.size()-1, "");
	if (e) {  t.ofs = i;  Redraw();  }

	auto fp = [](int i){  return f2s(100.f*i/16.f,0,3) + " %%";  };
	Sep(10);
	i = set.dimTabBck;  s = "Dim background: " + fp(i);  TextG(s);
	e = SliderInt("Tdb", &i, 1, 16, "");
	if (e) {  set.dimTabBck = i;  Redraw();  }

	i = set.dimTabTxt;  s = "Dim text: " + fp(i);  TextG(s);
	e = SliderInt("Tdt", &i, 1, 16, "");
	if (e) {  set.dimTabTxt = i;  Redraw();  }
	PopItemWidth();

/*	set.view.pls.xW_plS  todo: ...
	set.view.pls.bSlDrawR
	set.view.fnt.Fy = 17;  /// pls font size+
*/
//  app dim?
//	xSize = 390;  ySize = 900;  xPos = 0;  yPos = 0;
}
