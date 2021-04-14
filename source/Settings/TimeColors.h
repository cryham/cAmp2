#pragma once
#include <vector>
#include <string>


struct TimeClr  // color at time
{
	float t, r,g,b;
	
	TimeClr(float t1, float r1,float g1,float b1)
		: t(t1), r(r1),g(g1),b(b1)
	{	}
};


class TimeColors  // theme
{
	friend class ColorSets;
protected:
	std::vector<TimeClr> vc;
	std::string name;

public:
	TimeColors();

	//  get color for time, interpolate
	TimeClr Get(float time, int mode = 2);
};
