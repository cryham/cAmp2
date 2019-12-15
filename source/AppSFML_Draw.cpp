#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
using namespace std;


void AppSFMLDraw::DrawPlayer()
{
//	for (const auto& e:skin)
//		e.Draw();
	int xw = set.xwSize;
	
	if (audio->bFps)
	{
		Clr(120,160,240);
		str = "Fps: " + f2s(1.f / dt);
		bold = false;
		Text(10,10);
	}
	//bold = true;
	
	//  play info
	Clr(180,180,40);
	str = audio->IsPaused() ? "Pause" : audio->IsPlaying() ? "Play" : "Stop";
	Text(10,30);

	if (audio->IsPlaying())
	{
		Clr(150,190,240);
		audio->getPos();
		str = "Time: " + f2s(audio->timePlay,0) +
				" / " + f2s(audio->timeTrack,0);
		Text(90,30);

		Clr(120,160,240);
		str = "Vol: " + f2s(audio->fVolume);
		Text(xw-80,30);
	}
	
	//  cursors
	//Rect(0,60,xw,20, 0,70,241,10, 255,255,255);
	//Rect(0,80,xw,20, 0,90,251,12, 255,255,255);
	
	//  vis FFT
	if (audio->IsPlaying())
	{
		audio->GetVisData(xw);
		float* fft = audio->getFFT();

		//  params
		const float h = 190;
		const int uy=20 /*>0 darker*/, uh=112 /*112 line..fire 512*/;
		
		for (int i=0; i < xw; ++i)
		{
			float f = fft[i];
			int y = h - f*h;
			f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
			
			sf::Uint8 r,g,b;  // clr
			r = 40+f*f*(255-60);
			g = 100+f*(255-120);
			b = 180+f*(255-200);
			Rect(i, 90+h-y, 1,y, 475,uy,1,uh, r,g,b);

			//Rect(i, 90+h-y, 1,y, 32,180,1,10, 80,190,255);
			//Rect(i, 90+h-y, 1,y, 32,180,1,10, 40+f*f*(255-40),120+f*(255-120),255);
		}
	}
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
	pBackgr->setColor(sf::Color(r, g, b));
	pWindow->draw(*pBackgr);
}
