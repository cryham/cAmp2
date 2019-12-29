#pragma once
namespace tinyxml2 {  class XMLElement;  }

enum EVis
{
	viNone=0, viFFT, viOsc, viPrint, viALL
};

const int MaxViews = 8;


class ViewSet
{
public:
	ViewSet();
	void Defaults();

	void Load(const tinyxml2::XMLElement* e);
	void Save(tinyxml2::XMLElement* e);


	//  window
	int xSize,ySize, xPos,yPos;

	int iSleep;  // in ms
	bool bVSync;

	//  visualization
	int iVisH;  // draw height
	int iFFTSize;  // samples (fft quality)
	float fFFTMul;  // multiply y (scale)
	/*EVis*/ int eVis;
	float fPrtFq;  // voice print/spectrogram freq (speed)

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
