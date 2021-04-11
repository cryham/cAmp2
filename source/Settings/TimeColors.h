#pragma once
#include <vector>
#include <string>
#include "../System/LogFile.h"


struct TimeClr  // color at time
{
	float t, r,g,b;
	
	TimeClr(float t1, float r1,float g1,float b1)
		: t(t1), r(r1),g(g1),b(b1)
	{	}
};


class TimeColors  // theme
{
	friend class TimeClrSets;
protected:
	std::vector<TimeClr> vc;
	std::string name;

public:
	TimeColors();

	//  get color for time, interpolate
	TimeClr Get(float time, int mode = 2);
};


//  all themes
class TimeClrSets : public Logger
{
protected:
	std::vector<TimeColors> vt;
	int cur = 0;

public:
	TimeClrSets();

	bool Load(); //, Save();
	
	TimeClr Get(float time, int mode = 2);
};

