#pragma once
#include "VisualColors.h"
#include "FontSets.h"
#include "../App/AppConst.h"

namespace tinyxml2 {  class XMLElement;  class XMLDocument;  }


//  View settings
class ViewSet
{
public:
	ViewSet();
	void Defaults();

	void Load(const tinyxml2::XMLElement* e);
	void Save(tinyxml2::XMLElement* e, tinyxml2::XMLDocument* xml) const;

	std::string name;  // descr, with some info

	//  window  ------------------------
	struct VS_Wnd
	{
		int xSize = 300, ySize = 600;
		int xPos = 0, yPos = 0;

		int iSleep = 0;  // in ms
		bool bVSync = true;
	} wnd;
	
	//  visualizations  ------------------------
	const static int FFTSizes = 5;
	struct VS_Visuals
	{
		int yH = 130;  // draw height, top

		/*EVisType*/ int eType = VisT_FFT;
		
		struct VS_Vis_FFT
		{
			int iSize = 1;      // FFT samples (quality)
			float fMul = 69.f;  // y multiplier (scale)
			
			std::string theme;  // saved by name in xml <view>
			VisualColors clr;   // loaded from ColorSets outside by theme
		}
		fft, osc/*-*/, spect;
		
		SpectType eSpect;
		float fPrt_Fq = 100.f;  // spectrogram speed  // todo: thread
	} vis;
	
	//  pls slider  ------------------------
	struct VS_PlsSldr
	{
		bool drawRate = true;  // draw all rating on slider
		int width = 18;  // draw width, right
	} sldr;
	
	//  tabs counts: colums,rows, offset
	struct VS_Tabs
	{
		int xCols = 5, yRows = 1, ofs = 0;
	} tabs;
	
	//  fonts  ------------------------
	std::string fontSet;  // name for FontSets
	SFontSet fnt;   // loaded from FontSets outside by fontSet
	
	int FontH(EFont f) const
	{	return fnt.fnt[f].height;  }
	const SFont& Font(EFont f) const
	{	return fnt.fnt[f];  }
};
