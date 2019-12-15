#include "AppSFML_Draw.h"
using namespace std;  using namespace sf;


//  draw utils
//------------------------------------------------------------------

//  write text
int AppSFMLDraw::Text(int x, int y, bool draw)
{
	//if (!pWindow)  return;
	text.setString(str);
	text.setStyle(bold ? Text::Bold : Text::Regular);
	text.setColor(clr);
	text.setPosition(x, y);
	if (draw)  pWindow->draw(text);
	return static_cast<int>(text.getLocalBounds().width);  // advance x pos
}

//  draw rect
void AppSFMLDraw::Rect(int x, int y, int w, int h,
	int ux, int uy, int uw, int uh,
	Uint8 r, Uint8 g, Uint8 b)
{
	//if (!pBackgr)  return;
	pBackgr->setScale(float(w)/uw, float(h)/uh);  // stretch
	pBackgr->setTextureRect(IntRect(ux, uy, uw, uh));
	pBackgr->setPosition(x, y);
	pBackgr->setColor(Color(r, g, b));
	pWindow->draw(*pBackgr);
}

void AppSFMLDraw::Rect(int x, int y, int w, int h,  ETexUV uv,
	Uint8 r, Uint8 g, Uint8 b)
{
	const auto& t = ciTexUV[uv];
	Rect(x,y, w,h,  t.x,t.y, t.w,t.h,  r,g,b);
}
