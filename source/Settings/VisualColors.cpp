#include "VisualColors.h"
#include "../System/defines.h"
#include "../../libs/imgui.h"
#include <cmath>


void VisualColors::GetRGB(float f, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const
{
	float h,s,v;
	h = add.h + mul.h * powf(f, pow.h);
	s = add.s + mul.s * powf(f, pow.s);
	v = add.v + mul.v * powf(f, pow.v);
	h = fmod(h, 1.f);
	s = mia(0.f,1.f, s);
	v = mia(0.f,1.f, v);
	
	float rr,gg,bb;
	ImGui::ColorConvertHSVtoRGB(h,s,v, rr,gg,bb);
	r = rr*255.f;
	g = gg*255.f;
	b = bb*255.f;
}
