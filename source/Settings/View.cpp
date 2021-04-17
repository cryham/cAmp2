#include "View.h"
#include "VisualColors.h"
#include "../System/defines.h"  // mia
#include "../System/Utilities.h"
#include "../../libs/tinyxml2.h"
using namespace std;  using namespace tinyxml2;


ViewSet::ViewSet()
{
	Defaults();
}

void ViewSet::Defaults()
{
	//  for no cAmp2.xml or missing attributes
	wnd.xSize = 390;  wnd.ySize = 1143;
	wnd.xPos = 0;  wnd.yPos = 0;
	wnd.iSleep = 10;  wnd.bVSync = true;
	
	vis.yH = 129;
	vis.eType = VisT_FFT;
	vis.fft.iSize = 1;  vis.fft.fMul = 69.f;
	
	vis.spect.iSize = 2;  vis.spect.fMul = 69.f;
	vis.eSpect = SpcT_HorizRight;
	vis.fPrt_Fq = 100.f;
	
	auto& c = vis.fft.clr;
	c.name = "default view";
	c.add.h = 0.594f;  c.add.s = 0.893f;  c.add.v = 0.633f;
	c.mul.h =-0.099f;  c.mul.s =-1.033f;  c.mul.v = 0.273f;
	c.pow.h = 1.035f;  c.pow.s = 1.654f;  c.pow.v = 0.997f;
	vis.osc.clr = c;
	vis.spect.clr = c;
	
	pls.bSliderRate = 1;  pls.xW_slider = 18;
	
	tabs.xCols = 5;  tabs.yRows = 1;  tabs.ofs = 0;
	
	fnt.Fy = 17;  // todo:
	//cfP = 1;  cfA = 0;  cfT = 1;  cfH = 1;  cfG = 1;
}


///  . . .             Load             . . .
void ViewSet::Load(const XMLElement* el)
{
	Defaults();

	const char* a;  const XMLElement* e;
	
	a = e->Attribute("name");	if (a)  name = string(a);
	e = el->FirstChildElement("wnd");  if (e) {
		a = e->Attribute("sx");		if (a)  wnd.xSize = max(100, s2i(a));  //mia(100, ap->xScreen, cStr::toInt(a));
		a = e->Attribute("sy");		if (a)	wnd.ySize = max(30, s2i(a));  //mia(30,  ap->yScreen, cStr::toInt(a));
		a = e->Attribute("x");		if (a)  wnd.xPos = s2i(a);
		a = e->Attribute("y");		if (a)  wnd.yPos = s2i(a);

		a = e->Attribute("sleep");	if (a)  wnd.iSleep = s2i(a);
		a = e->Attribute("vsync");	if (a)  wnd.bVSync = s2b(a);
	}
	e = el->FirstChildElement("vis");  if (e) {
		a = e->Attribute("h");		if (a)  vis.yH = mia(0, wnd.ySize, s2i(a));
		a = e->Attribute("type");	if (a)  vis.eType = (EVisType)mia(0,int(VisT_ALL), s2i(a));
		
		a = e->Attribute("Fthm");	if (a)  vis.fft.theme = string(a);
		a = e->Attribute("Fsiz");	if (a)  vis.fft.iSize = mia(0,ViewSet::FFTSizes-1, s2i(a));
		a = e->Attribute("Fmul");	if (a)  vis.fft.fMul = s2f(a);

		a = e->Attribute("Othm");	if (a)  vis.osc.theme = string(a);
		
		a = e->Attribute("Sthm");	if (a)  vis.spect.theme = string(a);
		a = e->Attribute("Ssiz");	if (a)  vis.spect.iSize = mia(0,ViewSet::FFTSizes-1, s2i(a));
		a = e->Attribute("Smul");	if (a)  vis.spect.fMul = s2f(a);

		a = e->Attribute("Styp");	if (a)  vis.eSpect = (SpectType)mia(0,SpcT_ALL-1, s2i(a));
		a = e->Attribute("Sfrq");	if (a)  vis.fPrt_Fq = s2f(a);
	}
	e = el->FirstChildElement("pls");  if (e) {
		a = e->Attribute("slW");	if (a)  pls.xW_slider = max(0, s2i(a));

		a = e->Attribute("slRate");	if (a)  pls.bSliderRate = s2b(a);
	}
	e = el->FirstChildElement("tabs");  if (e) {
		a = e->Attribute("cols");	if (a)  tabs.xCols = max(1, s2i(a));
		a = e->Attribute("rows");	if (a)  tabs.yRows = max(0, s2i(a));
		a = e->Attribute("ofs");	if (a)  tabs.ofs = s2i(a);
	}
}

///  . . .             Save             . . .
void ViewSet::Save(XMLElement* el, XMLDocument* xml) const
{
	XMLElement* e;
	el->SetAttribute("name",		name.c_str());

	e = xml->NewElement("wnd");
		e->SetAttribute("sx",		i2s(wnd.xSize,4,' ').c_str());
		e->SetAttribute("sy",		i2s(wnd.ySize,4,' ').c_str());
		e->SetAttribute("x",		i2s(wnd.xPos,4,' ').c_str());
		e->SetAttribute("y",		i2s(wnd.yPos,4,' ').c_str());

		e->SetAttribute("sleep",	i2s(wnd.iSleep,2).c_str());
		e->SetAttribute("vsync",	b2s(wnd.bVSync).c_str());
	el->InsertEndChild(e);

	e = xml->NewElement("vis");
		e->SetAttribute("h",		i2s(vis.yH,4,' ').c_str());
		e->SetAttribute("type",		i2s(vis.eType).c_str());

		e->SetAttribute("fft",		i2s(vis.fft.iSize).c_str());
		e->SetAttribute("ftMul",	f2s(vis.fft.fMul).c_str());
		e->SetAttribute("vpFq",		f2s(vis.fPrt_Fq).c_str());

		e->SetAttribute("Fthm",		vis.fft.theme.c_str());
		e->SetAttribute("Fsiz",		i2s(vis.fft.iSize).c_str());
		e->SetAttribute("Fmul",		f2s(vis.fft.fMul).c_str());

		e->SetAttribute("Othm",		vis.osc.theme.c_str());

		e->SetAttribute("Sthm",		vis.spect.theme.c_str());
		e->SetAttribute("Ssiz",		i2s(vis.spect.iSize).c_str());
		e->SetAttribute("Smul",		f2s(vis.spect.fMul).c_str());

		e->SetAttribute("Styp",		i2s((int)vis.eSpect).c_str());
		e->SetAttribute("Sfrq",		f2s(vis.fPrt_Fq).c_str());

	el->InsertEndChild(e);

	e = xml->NewElement("pls");
		e->SetAttribute("sldrW",	i2s(pls.xW_slider,2).c_str());
		
		e->SetAttribute("sldrRate",	b2s(pls.bSliderRate).c_str());
	el->InsertEndChild(e);

	e = xml->NewElement("tabs");
		e->SetAttribute("cols",		i2s(tabs.xCols,2).c_str());
		e->SetAttribute("rows",		i2s(tabs.yRows).c_str());
		e->SetAttribute("ofs",		i2s(tabs.ofs).c_str());
	el->InsertEndChild(e);
}
