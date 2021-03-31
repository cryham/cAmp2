#include "View.h"
#include "def.h"  // mia
#include "AudioBass.h"  // const
#include "Util.h"

#include "../libs/tinyxml2.h"
using namespace std;  using namespace tinyxml2;


ViewSet::ViewSet()
{
	Defaults();
}

void ViewSet::Defaults()
{
	xSize = 390;  ySize = 900;  xPos = 0;  yPos = 0;
	iSleep = 10;  bVSync = true;
	
	iVisH = 116;  iFFTSize = 1;  fFFTMul = 69.f;
	eVis = viFFT;  fPrtFq = 100.f;
	
	bSlDrawR = 1;  xW_plS = 14;
	
	xNpt = 5;  yNpt = 1;/*2*/  ofsTab = 0;
	
	Fy = 17;  ///+
	//cfP = 1;  cfA = 0;  cfT = 1;  cfH = 1;  cfG = 1;
}


///  . . .             Load             . . .
void ViewSet::Load(const XMLElement* e)
{
	Defaults();

	const char* a;
	a = e->Attribute("sx");		if (a)  xSize = max(100, s2i(a));  //mia(100, ap->xScreen, cStr::toInt(a));
	a = e->Attribute("sy");		if (a)	ySize = max(30, s2i(a));  //mia(30,  ap->yScreen, cStr::toInt(a));
	a = e->Attribute("x");		if (a)  xPos = s2i(a);
	a = e->Attribute("y");		if (a)  yPos = s2i(a);

	a = e->Attribute("sleep");	if (a)  iSleep = s2i(a);
	a = e->Attribute("vsync");	if (a)  bVSync = s2b(a);

	a = e->Attribute("visH");	if (a)  iVisH = mia(0, ySize, s2i(a));
	a = e->Attribute("fft");	if (a)  iFFTSize = mia(0,ciFFTNum-1, s2i(a));
	a = e->Attribute("ftMul");	if (a)  fFFTMul = s2f(a);
	a = e->Attribute("eVis");	if (a)  eVis = (EVis)mia(0,int(viALL), s2i(a));
	a = e->Attribute("vpFq");	if (a)  fPrtFq = s2f(a);

	a = e->Attribute("sldr");	if (a)  xW_plS = max(0, s2i(a));
	a = e->Attribute("slR");	if (a)  bSlDrawR = s2b(a);

	a = e->Attribute("tbX");	if (a)  xNpt = max(1, s2i(a));
	a = e->Attribute("tbY");	if (a)  yNpt = max(0, s2i(a));
	a = e->Attribute("tbO");	if (a)  ofsTab = s2i(a);
}

///  . . .             Save             . . .
void ViewSet::Save(XMLElement* e)
{
	e->SetAttribute("sx",	i2s(xSize,4,' ').c_str());
	e->SetAttribute("sy",	i2s(ySize,4,' ').c_str());
	e->SetAttribute("x",	i2s(xPos,4,' ').c_str());
	e->SetAttribute("y",	i2s(yPos,4,' ').c_str());

	e->SetAttribute("sleep",	i2s(iSleep,2).c_str());
	e->SetAttribute("vsync",	b2s(bVSync).c_str());

	e->SetAttribute("visH",		i2s(iVisH,4,' ').c_str());
	e->SetAttribute("fft",		i2s(iFFTSize).c_str());
	e->SetAttribute("ftMul",	f2s(fFFTMul).c_str());
	e->SetAttribute("eVis",		i2s(eVis).c_str());
	e->SetAttribute("vpFq",		f2s(fPrtFq).c_str());

	e->SetAttribute("sldr",		i2s(xW_plS,2).c_str());
	e->SetAttribute("slR",		b2s(bSlDrawR).c_str());

	e->SetAttribute("tbX",	i2s(xNpt,2).c_str());
	e->SetAttribute("tbY",	i2s(yNpt).c_str());
	e->SetAttribute("tbO",	i2s(ofsTab).c_str());
}
