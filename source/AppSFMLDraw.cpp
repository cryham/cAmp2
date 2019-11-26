#include "AppSFMLDraw.h"
#include <iomanip>
using namespace std;


void AppSFMLDraw::DrawPlayer()
{
//	for (const auto& e:skin)
//		e.Draw();
	
	Clr(120,160,240);
	ostringstream ss;  ss.width(2);
	ss << fixed << setprecision(2) << 1.f / dt;
	str = "Fps: " + ss.str();
	bold = false;
	Text(10,10);
	bold = true;
	
	Clr(180,180,40);
	str = "Cool";
	Text(10,30);
	int xw = set.xwSize;
	
	Rect(0,60,xw,20, 0,70,241,10, 255,255,255);
	
	Rect(0,80,xw,20, 0,90,251,12, 255,255,255);
	
	//  vis fft fake
	for (int i=0; i < xw; ++i)
		Rect(i,110,1,rand()%190, 0,70,241,10, 255,255,255);
}


//  draw utils
//------------------------------------------------------------------

//  write text
int AppSFMLDraw::Text(int x, int y, bool draw)
{
	//if (!pWindow)  return;
	text.setString(str);
	text.setStyle(bold ? sf::Text::Bold : sf::Text::Regular);
	text.setColor(clr);
	text.setPosition(x, y);
	if (draw)  pWindow->draw(text);
	return static_cast<int>(text.getLocalBounds().width);  // advance x pos
}

//  draw rect
/*void AppSFMLDraw::Rect(int x, int y,  int u, int v, int w, int h,  const SClr& c)
{
	Rect(x, y,  u, v, w, h,  c.b, c.g, c.r);
}*/
void AppSFMLDraw::Rect(int x, int y, int w, int h,  int ux, int uy, int uw, int uh,
	sf::Uint8 r, sf::Uint8 g, sf::Uint8 b)
{
	//if (!pBackgr)  return;
	pBackgr->setScale(float(w)/uw, float(h)/uh);  // stretch
	pBackgr->setTextureRect(sf::IntRect(ux, uy, uw, uh));
	pBackgr->setPosition(x, y);
	//pBackgr->setColor(sf::Color(b, g, r));
	pWindow->draw(*pBackgr);
}
