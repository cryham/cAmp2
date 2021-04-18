#include "AppSFML_Draw.h"
#include "stdarg.h"
using namespace std;  using namespace sf;


void AppSFMLDraw::Format(const char* format, ...)
{
	const int strSize = 256;
	char s[strSize];
	va_list args;
	va_start(args, format);
	snprintf(s, strSize-1, format, args);
	s[strSize-1] = 0;
	str = s;
	va_end(args);
}

String AppSFMLDraw::Str(const std::string& s)
{
	return String::fromUtf8(s.begin(), s.end());
}

//  write text
int AppSFMLDraw::Text(EFont n, int x, int y, bool draw)
{
	//if (!pWindow)  return;
	text[n].setString(str);
	//text[n].setStyle(bold ? Text::Bold : Text::Regular);
	if (draw)  text[n].setColor(clr);
	text[n].setPosition(x, y);
	if (draw)  pWindow->draw(text[n]);
	return static_cast<int>(text[n].getLocalBounds().width);  // advance x pos
}

//  draw rect
void AppSFMLDraw::RectUV(int x, int y, int w, int h,
	int ux, int uy, int uw, int uh,  bool add,
	Uint8 r, Uint8 g, Uint8 b)
{
	//if (!pBackgr)  return;
	pBackgr->setScale(float(w)/uw, float(h)/uh);  // stretch
	pBackgr->setTextureRect(IntRect(ux, uy, uw, uh));
	pBackgr->setPosition(x, y);
	pBackgr->setColor(Color(r, g, b));
	if (add)
		pWindow->draw(*pBackgr, RenderStates(BlendAdd));
	else
		pWindow->draw(*pBackgr);
}

void AppSFMLDraw::Rect(int x, int y, int w, int h,
	ETexUV uv, bool add,
	Uint8 r, Uint8 g, Uint8 b)
{
	const auto& t = ciTexUV[uv];
	RectUV(x,y, w,h,  t.x,t.y, t.w,t.h, add,  r,g,b);
}
