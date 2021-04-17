#pragma once
#include "../App/AppConst.h"
#include <SFML/Config.hpp>  // Uint8
#include <string>


class Ratings
{
public:
	const static int cntMin = 3/*-*/, cntMax = 5/*+*/, cntAll = cntMin + cntMax + 1;
private:
    //  rating chars in name, for internal search
    const static int charsAll = 7;
    const static int valRates[charsAll];
	const static char charRates[charsAll];
	const static char* sCharRates;

	///  rating strings
    const static std::string sRateAdd[cntAll];
    const static std::string sRateVis[cntAll];
public:
    const static int valMin, valMax;

	//  methods
	static void Range(int& rate);
	
	inline static std::string GetAdd(int rate)  {	return sRateAdd[rate + cntMin];  }  // for filename
	inline static std::string GetVis(int rate)  {	return sRateVis[rate + cntMin];  }  // for drawing

	static void GetFromName(const std::string& name, int &pRate, int &pBookm);  // sets & if present
	static void CleanName(std::string& name);  // remove rating and bookmark chars

	static ETexUV GetTex(int rate);
	//  draw colors  // todo: in xml..
	const static sf::Uint8 clrTxt[cntAll][3], clrBck[cntAll][3];

	friend class Bookmarks;
};

class Bookmarks
{
public:
	const static int valMax;

	static void Range(int& bookm);
	static bool InRange(const char ch);

	static void GetFromName(const std::string& name, int& pBookm);  // sets & if present
};
