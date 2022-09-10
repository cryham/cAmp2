#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//------------------------------------------------------------------
//  App View
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_View()
{
	bool e;  string s;
	Checkbox("File Info (from cursor)", &set.bFileInfo);
	
	Sep(10);
	int i = set.eDirView;
	if (SliderI(i, 0, DirV_ALL-1, "Dir View: ", "dirv", csDirView[i]))
	{	set.eDirView = (EDirView)i;  Redraw();  }

	Sep(10);
	if (Checkbox("Slider draw all ratings", &set.view.sldr.drawRate))  Redraw();

	Sep(5);
	if (SliderI(set.view.sldr.width, 2, 60, "Slider width: ", "sldW"))  UpdDim();

	Sep(10);  Line();  e = false;
	
	TextG("Font sizes:");	xSlider = 200;
	for (int i=0; i < themes.FontCount(); ++i)
	{
		s = themes.GetFontName(i)+"###FntSet"+i2s(i);
		if (Button(s.c_str()))
		{
			themes.curFonts = i;
			set.view.fontSet = themes.CurFont().name;
			set.view.fnt = themes.CurFont();
			LoadFonts();
			UpdDim();
		}
		if (i%3 < 2)  SameLine();  // 3 in row
	}

	xSlider = 0;
	Sep(5);
	e = Button("Edit fonts..");  if (e)  wndOpen = WO_ViewFonts;
}


//  Fonts
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_ViewFonts()
{
	bool e;  string s;
	Sep(5);	 xSlider = 200;
	TextG("Adjust fonts");

	for (int i=0; i < Fnt_ALL; ++i)
	{
		auto& f = set.view.fnt.fnt[i];
		TextG(csFonts[i]);
		PushItemWidth(50);
		s = " + ###F+"+i2s(i);  SameLine(200);  if (Button(s.c_str())) {  e = true;  ++f.size;  ++f.height;  }
		s = " - ###F-"+i2s(i);  SameLine(280);  if (Button(s.c_str())) {  e = true;  --f.size;  --f.height;  }
		PopItemWidth();
		e |= SliderI(f.size, 1, 40, "  Size: ", "fntS" + i2s(i));
		if (i == Fnt_Tabs || i == Fnt_Tracks)  // rest unused
			e |= SliderI(f.lineSpacing, -5, 20, "  Spacing: ", "fntH" + i2s(i));
	}
	if (e)
	{	LoadFonts();  UpdDim();  }

	xSlider = 0;
	Sep(5);
	if (Button("Back to View.."))  wndOpen = WO_View;
}


//  Times
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_ViewTimes()
{
	
}
