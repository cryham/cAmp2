#pragma once
namespace tinyxml2 {  class XMLElement;  }

enum EVis
{	viNone=0, viFFT, viOsc, viPrint, viALL  };


class ViewSet
{
public:
	ViewSet();
	void Defaults();

	void Load(const tinyxml2::XMLElement* e);
	void Save(tinyxml2::XMLElement* e);


	//  window
	int xSize = 300, ySize = 600;
	int xPos = 0, yPos = 0;

	int iSleep = 0;  // in ms
	bool bVSync = true;

	//  visualization
	int iVisH = 96;  // draw height
	int iFFTSize = 1;      // FFT samples (quality)
	float fFFTMul = 69.f;  // y multiplier (scale)
	/*EVis*/ int eVis = viFFT;  // type
	float fPrtFq = 100.f;  // spectrogram speed

	//  slider
	bool bSlDrawR;  // draw rating
	int xW_plS;  // width
	
	//  tabs count
	int xNpt, yNpt, ofsTab;

	//  font sizes
	int Fy;
	//int cfP,cfA,cfT,cfH,cfG;  // pls,amp,tabs,hlp,gui

	// CList::bFilInf  CList::iDirView
};
