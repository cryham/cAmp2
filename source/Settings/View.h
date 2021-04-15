#pragma once
#include "VisualColors.h"

namespace tinyxml2 {  class XMLElement;  class XMLDocument;  }


enum EVisType
{	VisT_None, VisT_FFT, VisT_Osc, VisT_Spect, VisT_ALL  };
const static char* csVisType[VisT_ALL] =
{	"None", "FFT", "Oscilloscope", "Spectrogram" };

enum SpectType 
{	SpcT_VerticalDown, SpcT_HorizRight, SpcT_VerticalUpFFT, SpcT_ALL  };
const static char* csSpectType[SpcT_ALL] =
{	"Vertical Down", "Horizontal Right", "Vertical Up with FFT"  };


//  View settings
class ViewSet
{
public:
	ViewSet();
	void Defaults();

	void Load(const tinyxml2::XMLElement* e);
	void Save(tinyxml2::XMLElement* e, tinyxml2::XMLDocument* xml) const;


	//  window
	struct VS_Wnd {
		int xSize = 300, ySize = 600;
		int xPos = 0, yPos = 0;

		int iSleep = 0;  // in ms
		bool bVSync = true;
	} wnd;
	
	//  visualizations
	const static int FFTSizes = 5;
	struct VS_Visuals {
		int yH = 130;  // draw height, top

		/*EVisType*/ int eType = VisT_FFT;
		
		struct VS_Vis_FFT
		{
			int iSize = 1;      // FFT samples (quality)
			float fMul = 69.f;  // y multiplier (scale)
			VisualColors clr;
			int id = 0;  // todo: only id saved in xml
		} fft, osc/*-*/, spect;
		
		SpectType eSpect;
		float fPrt_Fq = 100.f;  // spectrogram speed  // todo: thread
	} vis;
	
	//  pls slider
	struct VS_Pls {
		bool bSliderRate = 1;  // draw rating

		int xW_slider = 18;  // draw width, right
	} pls;
	
	//  tabs counts: colums,rows, offset
	struct VS_Tabs {
		int xCols = 5, yRows = 1, ofs = 0;
	} tabs;
	
	//  font sizes
	struct VS_Fonts {
		int Fy = 17;
	} fnt;
};
