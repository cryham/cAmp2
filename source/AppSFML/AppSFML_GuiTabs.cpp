#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//------------------------------------------------------------------
///  Pls Tab
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_Tab()
{
	bool e;  string s;  int i;
	static char t[1024]={0};

	strcpy(t, Pls().name.c_str());  TextG("Name:");
	PushItemWidth(300);  WndFocus();
	e = InputText("Tbn", t, sizeof(t));
	if (e) {  Pls().name = t;  Redraw();  }

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
void AppSFMLDraw::WndDraw_TabsAll()
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

	Sep(10);
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

//  app dim?
//	xSize = 390;  ySize = 900;  xPos = 0;  yPos = 0;
}
