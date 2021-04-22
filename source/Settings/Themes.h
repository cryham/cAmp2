#pragma once
#include "TimeColors.h"
#include "VisualColors.h"
#include "FontSets.h"
#include "../System/LogFile.h"
#include <vector>
#include <string>
#include <map>


//  all themes: colors sets, fonts etc  from themes.xml

class Themes : public Logger
{
protected:
	std::vector<TimeColors> vTime;

	std::vector<VisualColors> vVisual;
	std::map<std::string, int> mapVisual;  // id to vVisual by name

	std::vector<SFontSet> vFonts;
	std::map<std::string, int> mapFonts;  // id to vFonts by name
public:
	//  note vectors need at least 1, default
	int curTime = 0;

	int curFFT = 0;  // id to vVisual for all 3 visual types
	int curOsc = 0;
	int curSpect = 0;

	int curFonts = 0;
	
public:
	Themes();

	//  main, xml
	void Defaults();
	bool Load(), Save() const;
	
	//  time
	TimeClr GetTimeClr(float time, int mode = 2);
	
	//  vis
	int GetVisId(const std::string &name);  // id by name
	int VisCount() const {  return (int)vVisual.size();  }
	
	VisualColors& CurFFT()	{	return vVisual[curFFT];  }
	VisualColors& CurOsc()	{	return vVisual[curOsc];  }
	VisualColors& CurSpect(){	return vVisual[curSpect];  }

	//  fonts
	int GetFontId(const std::string &name);  // id by name
	int FontCount() const {  return (int)vFonts.size();  }
	
	SFontSet& CurFont() {  return vFonts[curFonts];  }
	std::string GetFontName(int id)	{	return vFonts[id].name;  }
};
