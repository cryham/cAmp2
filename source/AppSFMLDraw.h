#pragma once
#include "App.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
//#include "List.h"


//  Color
struct SClr
{
	typedef unsigned char byte;
	byte r,g,b;  // color
	//float h,s,v;  //todo: use,save hsv

	SClr()
		:r(255),g(255),b(255)
	{	}
	SClr(byte rr, byte gg, byte bb)
		:r(rr),g(gg),b(bb)
	{	}

	void Set(sf::Uint32 u);
	sf::Uint32 Get();

	bool operator!=(const SClr& c) const
	{
		return r != c.r || g != c.g || b != c.b;
	}
};

//------------------------------------------------
class AppSFMLDraw //: public App
{
	//  window and resources
	//--------------------------
	std::unique_ptr<sf::RenderWindow> pWindow = nullptr;
	std::unique_ptr<sf::Sprite> pBackgr = nullptr;

	std::unique_ptr<sf::Font> pFont = nullptr;
	sf::Text text;

public:
	sf::String str;
	sf::Color clr;
	bool bold = false;

	//  set text color
	//--------------------------
	void Clr(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b)
	{
		clr = sf::Color(r,g,b);
	}
	void Clr(const SClr& c)
	{
		clr = sf::Color(c.r, c.g, c.b);
	}

	//  write out text, from str
	//  returns width, x advance
	int Txt(int x, int y, bool draw=true);

	//  clear rect
	void Rect(int x, int y,  int sx, int sy,  const SClr& c);
	void Rect(int x, int y,  int sx, int sy,  sf::Uint8 r, sf::Uint8 g, sf::Uint8 b);
	
	//  frame rect
	void Frame(int x, int y,  int sx, int sy,  int d,  const SClr& c);
	void Frame(int x, int y,  int sx, int sy,  int d,  sf::Uint8 r, sf::Uint8 g, sf::Uint8 b);
	void Frame(int x, int y,  int sx, int sy,  int d,  bool le, bool ri,  const SClr& c);
	void Frame(int x, int y,  int sx, int sy,  int d,  bool le, bool ri,  sf::Uint8 r, sf::Uint8 g, sf::Uint8 b);

public:
	bool Run();
};
