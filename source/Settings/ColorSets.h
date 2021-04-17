#pragma once
#include "TimeColors.h"
#include "VisualColors.h"
#include "../System/LogFile.h"
#include <vector>
#include <string>
#include <map>


//  all color sets
//  themes from colors.xml

class ColorSets : public Logger
{
protected:
	std::vector<TimeColors> vTime;

	std::vector<VisualColors> vVisual;
	std::map<std::string, int> mapVisual;  // id to vVisual by name
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
	
	//  time
	TimeClr Get(float time, int mode = 2);
	
	//  vis
	int GetVis(const std::string &name);  // id by name
	int VisCount() const {  return (int)vVisual.size();  }
	
	VisualColors& CurFFT()	{	return vVisual[curFFT];  }
	VisualColors& CurOsc()	{	return vVisual[curOsc];  }
	VisualColors& CurSpect(){	return vVisual[curSpect];  }
};
