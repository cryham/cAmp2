#pragma once
#include "TimeColors.h"
#include "VisualColors.h"
#include "../System/LogFile.h"
#include <vector>
#include <string>


//  all color sets
//  themes from colors.xml

class ColorSets : public Logger
{
public:  //protected:
	std::vector<TimeColors> vTime;

	std::vector<VisualColors> vVisual;
public:
	int curTime = 0;

	int curFFT = 0;  // id to vVisual for all 3 visual types
	int curOsc = 0;
	int curSpect = 0;

public:
	ColorSets();

	//  main, xml
	void Defaults();
	bool Load(), Save() const;
	
	//  utils
	TimeClr Get(float time, int mode = 2);
	
	VisualColors& CurFFT()	{	return vVisual[curFFT];  }
	VisualColors& CurOsc()	{	return vVisual[curOsc];  }
	VisualColors& CurSpect(){	return vVisual[curSpect];  }
};
