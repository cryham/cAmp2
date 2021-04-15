#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/defines.h"
#include <cmath>
using namespace std;  using namespace sf;


///  Visualizations  ~ ~ ~ ~ ~
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawVisual()
{
	const auto& v = set.view;  //const auto& vis = set.view.vis;
	const int xw = v.wnd.xSize, yw = v.wnd.ySize;
	const bool play = audio->IsPlaying();
	const float h = yE_vis - yB_vis;
	const auto t = v.vis.eType;
	const int hh = v.vis.yH, yh = hh;

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
				v.vis.fft.clr.GetRGB(f, r,g,b);
				RectUV(i, yB_vis + h-y, 1,y,  475,uy,1,uh, true, r,g,b);
				// todo: fill 1d texture and use shader
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
				v.vis.osc.clr.GetRGB(f, r,g,b);
//				r = 20+f*f*(255-90);
//				g = 80+f*(255-120);
//				b = 180+f*(255-200);
				RectUV(i, yB_vis + ya, 1,d,  475,uy,1,uh, true, r,g,b);
				// todo: fill 1d texture and use shader, add glow
			}
		///------------------------------------------------------------------------------------
		else if (t == VisT_Spect && pVisSprite)
		{
			bool right = v.vis.eSpect == SpcT_HorizRight;
			bool withFFT = v.vis.eSpect == SpcT_VerticalUpFFT;

			// draw FFT  split h
			//hh -= hh/3;  // 1/3 fft, rest spect
			const int h2 = hh / 3, hh2 = hh-h2;
			
			if (withFFT)
			for (int i=0; i < xw; ++i)
			{
				float f = vis[i];
				int y = h2 - f*h2;
				f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
				
				Uint8 r,g,b;  // clr
				v.vis.spect.clr.GetRGB(f, r,g,b);
				RectUV(i, yB_vis + h2-y, 1,y,  475,uy,1,uh, true, r,g,b);
			}
			
			Uint8 pixels[ciMaxScreenX * 4];
			static int yy = 0;  int a=0;
			static int xx = 0;
			
			if (right)
			{
				for (int j=0; j < yw; ++j)
				{
					float f = vis[yh-1-j];
					int y = h - f*h;
					f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
					
					Uint8 r,g,b;  // clr
					v.vis.spect.clr.GetRGB(f, r,g,b);
					pixels[a++] = r;  pixels[a++] = g;  pixels[a++] = b;  pixels[a++] = 255;
				}
				pVisTexture->update(pixels, 1, yw, xx, 0);
			}else{
				for (int i=0; i < xw; ++i)
				{
					float f = vis[i];
					int y = h - f*h;
					f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
					//f = float(i)/xw * 0.5f + 1.f * abs(yy-hh/2)/hh;  // test
					
					Uint8 r,g,b;  // clr
					v.vis.spect.clr.GetRGB(f, r,g,b);
				#if 0  //  test ::
					r = (i%2==0?1:0) * 222;  //255.f * i/xw;
					g = (yy%2==0?1:0) * 100 + 100.f * i/xw;
					b = 155.f * yy/hh + 100.f * i/xw;
					//if (yy == hh-1)  r = g = b = 255;
				#endif
					pixels[a++] = r;  pixels[a++] = g;  pixels[a++] = b;  pixels[a++] = 255;
				}
				pVisTexture->update(pixels, xw, 1, 0, yy);
			}
			pVisTexture->setRepeated(true);
			//pVisTexture->setSmooth(false);
			
			pVisSprite->setScale(1.f, 1.f);
			pVisSprite->setColor(Color(255, 255, 255));
		#if 0
			pVisSprite->setTextureRect(IntRect(0, 0, xw, h));  // static
			pWindow->draw(*pVisSprite);
		#else
			//  bRolling par..
			const int yb = yB_vis + 10;  // top
			switch (v.vis.eSpect)
			{
			case SpcT_VerticalUpFFT:  // todo: mirror
				pVisSprite->setTextureRect(IntRect(0, yy+1, xw, hh2-1-yy));
				pVisSprite->setPosition(0, yb +h2);
				pWindow->draw(*pVisSprite);
				pVisSprite->setTextureRect(IntRect(0, 0, xw, yy));
				pVisSprite->setPosition(0, yb +h2 +hh2-1-yy);
				pWindow->draw(*pVisSprite);  break;
			case SpcT_VerticalDown:
				pVisSprite->setTextureRect(IntRect(0, yy+1, xw, hh-1-yy));
				pVisSprite->setPosition(0, yb);
				pWindow->draw(*pVisSprite);
				pVisSprite->setTextureRect(IntRect(0, 0, xw, yy));
				pVisSprite->setPosition(0, yb +hh-1-yy);
				pWindow->draw(*pVisSprite);  break;
			case SpcT_HorizRight:
				pVisSprite->setTextureRect(IntRect(xx+1, 0, xw-1-xx, yh));
				pVisSprite->setPosition(0, yb);
				pWindow->draw(*pVisSprite);
				pVisSprite->setTextureRect(IntRect(0, 0, xx, yh));
				pVisSprite->setPosition(xw-1-xx, yb);
				pWindow->draw(*pVisSprite);  break;
			}
		#endif
			if (!right)
			{	++yy;  if (yy >= (withFFT ? hh2 : hh))  yy = 0;  }else
			{	++xx;  if (xx >= xw)  xx = 0;  }
		}
	}	
}
