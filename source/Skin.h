#pragma once
#include <vector>
#include "SkinElement.h"


class Skin
{
protected:
public:

	std::vector<SkinElement> el;

public:
    Skin();
	
	bool Load();

};
