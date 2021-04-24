#include "AppSFML_Draw.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//------------------------------------------------------------------
//  Visualization
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_Vis()
{
	bool e;  string s;  float f;  int i, x = 300;
	auto& v = set.view.vis;
	xText = 140;  xSlider = 240;

	//  view name edit
	TextG("View name: ");  SameLine(xSlider);
	static char ed[1024]={0};
	auto& ss = set.views[iLastView].name;
	strcpy(ed, ss.c_str());  WndFocus();
	PushItemWidth(x);  e = InputText("View name", ed, sizeof(ed));  PopItemWidth();
	if (e)  ss = ed;

	SameLine(xSlider+x+30);		if (Button("Load"))  UpdateView(true, iLastView);
	SameLine(xSlider+x+150);	if (Button("Save"))  UpdateView(false, iLastView);
	
	//  view num
	PushItemWidth(650);
	SliderI(iLastView, 0, Settings::cMaxViews-1, "View number: ", "vwNum");
	
	Sep(10);
	if (SliderI(v.eType, 0, VisT_ALL-1, string("Type: "), "visT", csVisType[v.eType]))
		UpdDim();
	
	//  / set.view.wnd.ySize;  %
	if (SliderI(v.yH, 0, set.view.wnd.ySize, string("Height: "), "visH"))
		UpdDim();

	//	int iSleep = 0;  // in ms  // todo:
	//	bool bVSync = true;
	
	const int clrs = themes.VisCount()-1;
	Sep(10);
	switch (v.eType)
	{
	case VisT_FFT:
	{
		SliderI(v.fft.iSize, 0, ViewSet::FFTSizes-1,
				"FFT size: ", "fft-siz");
	
		SliderF(v.fft.fMul, 0.f, 200.f,
				"FFT scale: ", "fft-mul");

		if (SliderI(themes.curFFT, 0, clrs,
				"Theme: " + i2s(themes.curFFT), "fft-thm", themes.CurFFT().name))
		{	v.fft.clr = themes.CurFFT();  v.fft.theme = themes.CurFFT().name;  }
		
		if (Button("Edit theme.."))  wndOpen = WO_VisThemes;
	}	break;

	case VisT_Osc:
	{
		if (SliderI(themes.curOsc, 0, clrs,
				"Theme: " + i2s(themes.curOsc), "osc-thm", themes.CurOsc().name))
		{	v.osc.clr = themes.CurOsc();  v.osc.theme = themes.CurOsc().name;  }
		
		if (Button("Edit theme.."))  wndOpen = WO_VisThemes;
	}	break;

	case VisT_Spect:
	{
		SliderI(v.spect.iSize, 0, ViewSet::FFTSizes-1,
				"FFT size: ", "spc-siz");
	
		SliderF(v.spect.fMul, 0.f, 200.f,
				"FFT scale: ", "spc-mul");

		if (SliderI(themes.curSpect, 0, clrs,
				"Theme: " + i2s(themes.curSpect), "spc-thm", themes.CurSpect().name))
		{	v.spect.clr = themes.CurSpect();  v.spect.theme = themes.CurSpect().name;  }

		i = v.eSpect;
		if (SliderI(i, 0, SpcT_ALL-1,
				"Type: ", "spc-typ", string(csSpectType[i])))
			v.eSpect = (SpectType)i;
		
		if (Button("Edit theme.."))  wndOpen = WO_VisThemes;

		//	float fPrtFq = 100.f;  // spectrogram speed
	}	break;
	}

	PopItemWidth();  xText = 0;  xSlider = 0;
}


//  Themes
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_VisThemes()
{
	bool e;  string s;  float f;  int i, x = 300;
	auto& v = set.view.vis;
	xText = 140;  xSlider = 240;
	PushItemWidth(650);
	
	auto AddSlidersHSV = [&](VisualColors& c, string s)
	{
		/*{	//  preview gradient ..
			ImVec2 p0 = GetCursorScreenPos();
			ImVec2 gradient_size = ImVec2(CalcItemWidth(), GetFrameHeight());
			ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
			ImU32 col_a = GetColorU32(IM_COL32(0, 255, 0, 255));
			ImU32 col_b = GetColorU32(IM_COL32(255, 0, 0, 255));
			ImDrawList* draw_list = GetWindowDrawList();
			draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
			InvisibleButton("##gradient2", gradient_size);
		}/**/
		Sep(10);
		TextG("Adjust theme coloring");
		Sep(5);  //Line();
		SliderF(c.add.h, 0.f, 1.f, "Hue: ",        s+"Ha");
		SliderF(c.mul.h,-2.f, 2.f, "  multi: ",    s+"Hm");
		SliderF(c.pow.h, 0.f, 4.f, "  power: ",    s+"Hp");
		Sep(5);
		SliderF(c.add.s,-1.f, 2.f, "Saturation: ", s+"Sa");
		SliderF(c.mul.s,-3.f, 3.f, "  multi: ",    s+"Sm");
		SliderF(c.pow.s, 0.f, 4.f, "  power: ",    s+"Sp");
		Sep(5);
		SliderF(c.add.v,-1.f, 2.f, "Brightness: ", s+"Va");
		SliderF(c.mul.v,-3.f, 3.f, "  multi: ",    s+"Vm");
		SliderF(c.pow.v, 0.f, 4.f, "  power: ",    s+"Vp");
		Sep(5);
		if (Button("Back to Visual.."))  wndOpen = WO_Vis;
	};

	const int clrs = themes.VisCount()-1;
	Sep(10);
	switch (v.eType)
	{
	case VisT_FFT:
	{
		if (SliderI(themes.curFFT, 0, clrs,
				"Theme: " + i2s(themes.curFFT), "fft-thm", themes.CurFFT().name))
		{	v.fft.clr = themes.CurFFT();  v.fft.theme = themes.CurFFT().name;  }

		// todo: btn set cur, save as name..
		
		AddSlidersHSV(v.fft.clr, "fft_");
	}	break;

	case VisT_Osc:
	{
		if (SliderI(themes.curOsc, 0, clrs,
				"Theme: " + i2s(themes.curOsc), "osc-thm", themes.CurOsc().name))
		{	v.osc.clr = themes.CurOsc();  v.osc.theme = themes.CurOsc().name;  }
		
		AddSlidersHSV(v.osc.clr, "osc_");
	}	break;

	case VisT_Spect:
	{
		if (SliderI(themes.curSpect, 0, clrs,
				"Theme: " + i2s(themes.curSpect), "spc-thm", themes.CurSpect().name))
		{	v.spect.clr = themes.CurSpect();  v.spect.theme = themes.CurSpect().name;  }

		AddSlidersHSV(v.spect.clr, "spc_");
	}	break;
	}
	PopItemWidth();  xText = 0;  xSlider = 0;
}
