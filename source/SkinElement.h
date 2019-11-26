#pragma once

#include <string>


class SkinElement
{
protected:

	std::string name;

public:
	enum Type
	{
		SE_Text, SE_Rect,
		//SE_Fps=0,
		//SE_PlayInfo, SE_TimeCur, SE_TimeTotal,
		SE_Visualization,
		SE_LAST
	} type = SE_Text;

public:
	SkinElement();
	
	virtual void Draw() = 0;

};
