#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/defines.h"
#include <cmath>
using namespace std;  using namespace sf;


///  Visualizations  ~ ~ ~ ~ ~
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawVisual()
{
	const ViewSet& v = set.view;
	const int xw = v.wnd.xSize, yw = v.wnd.ySize, hh = v.vis.yH;
	const bool play = audio->IsPlaying();
	const float h = yE_vis - yB_vis;
	const auto t = v.vis.eType;

	if (play)
	{
		audio->GetVisualData(xw, v);
		const float* vis = audio->GetVisual();
		const int uy=40 /*>0 darker*/, uh=52 /*0 line..fire 512*/;  // params

		if (t == VisT_FFT)
			for (int i=0; i < xw; ++i)
			{
				float f = vis[i];
				int y = h - f*h;
				f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
				
				Uint8 r,g,b;  // clr
				r = 40+f*f*(255-60);
				g = 100+f*(255-120);
				b = 180+f*(255-200);
				RectUV(i, yB_vis + h-y, 1,y,  475,uy,1,uh, true, r,g,b);
				// todo: fill texture and use shader
			}
		else if (t == VisT_Osc)
			for (int i=0; i < xw; ++i)
			{
				float f = vis[i];
				int y1 = h - f*h;
				int y2 = h - vis[i+1]*h;
				int ya = min(y1,y2), yb = max(y1,y2);

				int d = yb - ya + 1;  // darker with high tones
				float y = /*0.5f -*/ fabs(f - 0.5f);  // lighter at high amplitudes
				//f = mia(0.f,1.f, 0.4f - y*0.16f + d*0.01f);  // mul
				f = mia(0.f,1.f, 0.8f + y*1.0f - d*0.02f);  // mul  d/h  par +
				
				Uint8 r,g,b;  // clr
				r = 20+f*f*(255-90);
				g = 80+f*(255-120);
				b = 180+f*(255-200);
				RectUV(i, yB_vis + ya, 1,d,  475,uy,1,uh, true, r,g,b);
			}
		else if (t == VisT_Spect && pVisSprite)
		{
			Uint8 pixels[xw*4];
			static int yy = 0;
			for (int i=0; i < xw; ++i)
			{
				float f = vis[i];
				int y = h - f*h;
				f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
				//f = float(i)/xw * 0.5f + 1.f * abs(yy-hh/2)/hh;  // test
				
				Uint8 r,g,b;  // clr
				r = 10+f*f*(255-100);
				g = 10+f*(255-60);
				b = 40+f*(255-40);
			#if 0  //  test ::
				r = (i%2==0?1:0) * 222;  //255.f * i/xw;
				g = (yy%2==0?1:0) * 100 + 100.f * i/xw;
				b = 155.f * yy/hh + 100.f * i/xw;
				//if (yy == hh-1)  r = g = b = 255;
			#endif
				int a = i*4;
				pixels[a++] = r;
				pixels[a++] = g;
				pixels[a++] = b;
				pixels[a++] = 255;
			}
			pVisTexture->update(pixels, xw, 1, 0, yy);
			pVisTexture->setRepeated(true);
			//pVisTexture->setSmooth(false);
			
			pVisSprite->setScale(1.f, 1.f);
			pVisSprite->setColor(Color(255, 255, 255));
		#if 0
			pVisSprite->setTextureRect(IntRect(0, 0, xw, h));  // static
			pWindow->draw(*pVisSprite);
		#else
			//  rolling  // par..  horiz/vert..
			pVisSprite->setTextureRect(IntRect(0, yy+1, xw, hh-1-yy));
			pVisSprite->setPosition(0, yB_vis+10);
			pWindow->draw(*pVisSprite);
			pVisSprite->setTextureRect(IntRect(0, 0, xw, yy));
			pVisSprite->setPosition(0, yB_vis+10 +hh-1-yy);
			pWindow->draw(*pVisSprite);
		#endif
			++yy;
			if (yy >= hh)
				yy = 0;
		}
	}	
}
