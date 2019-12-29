#pragma once
class XMLElement;


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

	void Load(const XMLElement* m);
	void Save(XMLElement* m);


	//  window
	int xSize,ySize, xPos,yPos;

	int iSleep;  // in ms
	bool vsync;

	//  visualization
	int visH;  // draw height
	int fftSize;  // samples (fft quality)
	float fftMul;  // multiply y (scale)
	/*EVis*/ int eVis;
	float pr_fq;  // voice print/spectrogram freq (speed)

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
