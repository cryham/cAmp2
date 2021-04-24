#pragma once
#include "../App/AppConst.h"
#include <vector>
#include <string>
#include <map>


struct SFont
{
	std::string name = "";
	bool bold = false;
	int size = 15;  // font size
	int lineSpacing = 3;  // add to height
	int height = 18;  // line height = size + lineSpacing  (auto set)
};

struct SFontSet
{
	std::string name = "";

	SFont fnt[Fnt_ALL];
	
	SFontSet();
	void Default();
};
