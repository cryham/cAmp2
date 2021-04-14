#pragma once
#include <SFML/Graphics.hpp>
#include <string>


struct Float3HSV  // color H,S,V
{
	float h = 0.f, s = 0.f, v = 1.f;
};


class VisualColors  // visualization theme
{
public:
	std::string name;
	Float3HSV add, mul, pow;  // value, multiplier, power

	//  get color for factor
	void GetRGB(float f, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const;
};
