#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <cmath>
using namespace std;  using namespace sf;


//  Draw all
void AppSFMLDraw::Draw()
{
	if (pls->bDraw)
	{	pls->bDraw = false;
		bDraw = true;
	}
	if (bDraw)
		pWindow->clear();
	
	DrawPlayer();
	
	if (bDraw)
	{	bDraw = false;
		
		DrawPlaylist();
	}
}


//  Player  --
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPlayer()
{
	//for (const auto& e:skin)
	//	e.Draw();
	
	const ViewSet& v = set.view;
	int xw = v.xSize, yw = v.ySize;
	bool play = audio->IsPlaying();

	
	//  backgr  []
	Rect(0,0, xw,yE_vis, TX_BackPlr, false);
	
	
	//  Vis FFT  ~~~~~
	//  params
	int h = yE_vis - yB_vis;
	const int uy=40 /*>0 darker*/, uh=52 /*0 line..fire 512*/;

	if (v.eVis == viFFT && play)
	{
		audio->GetVisData(xw, v);
		float* fft = audio->getFFT();

		for (int i=0; i < xw; ++i)
		{
			float f = fft[i];
			int y = h - f*h;
			f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
			
			Uint8 r,g,b;  // clr
			r = 40+f*f*(255-60);
			g = 100+f*(255-120);
			b = 180+f*(255-200);
			RectUV(i, yB_vis + h-y, 1,y,  475,uy,1,uh, true, r,g,b);
		}
	}
	
	
	//  Position bar  --=---
	int yH = yE_pos - yB_pos;
	Rect(0.f,yB_pos, xw,yH, TX_PosDk, false);

	if (play)
	{
		audio->getPos();
		float xp = (audio->timePlay / audio->timeTrack) * (1.f-xW_pos), //marg,bar dim
			x1 = xp*xw;  int xb = xW_pos*xw;
		
		const auto& t = ciTexUV[TX_PosBr];
		RectUV(x1, yB_pos,  xb, yH, 
			 t.x+t.w*0.48f, t.y,  t.w*0.04f, t.h);
	}

	
	if (bFps)
	{
		Clr(120,160,240);
		str = f2s(1.f / dt);
		Text(Fnt_Info, 60, 16);
	}

	
	//  play icons  >
	int y = 2;
	Clr(50,90,120);
	//str = audio->IsPaused() ? "||" : play ? "|>" : "[]";  // simple
	str = audio->IsPaused() ? String(Uint32(0x25AE)) + String(Uint32(0x25AE))
		: play ? String(Uint32(0x25B6)) : String(Uint32(0x25A0));  // ▮▮ ▶ ■
	Text(Fnt_Info, xw/2, y);
	
	//  repeat  @
	if (audio->bRepTrk || audio->bRepPls)
	{
		Clr(50,100,100);
		str = String(Uint32(0x21BB));  // ↻
		if (audio->bRepTrk)  str += "1";
		//else  str += "A";
		Text(Fnt_Info, xw-75, y+25);  // +Fy TimeBig..
	}
	

	//  player  info text  * * *
	if (play)
	{
		//  ext bitrate freq size
		Clr(110,140,190);
		str = audio->sInfo;
		Text(Fnt_Info, 10, y);
				
		//  time right align..
		str = "0";
		const int w0 = Text(Fnt_TimeBig, 0,0, false);
		string s;  int l, w;

		//  Time cur
		Clr(120,160,220);
		bool below10 = audio->timeTrack < 10.0;
		s = t2s(audio->timePlay, below10);  str = s;
		l = s.length();  w = l * w0;
		Text(Fnt_TimeBig, xw - w - 110, y-2);

		//  Time track
		Clr(100,140,210);
		s = t2s(audio->timeTrack);  str = s;
		l = s.length();  w = l * w0;
		Text(Fnt_TimeBig, xw - w - 20, y-2);
		
		//  track rating
		int rr = audio->rate, r = rr+3;
		str = chFRateVis[r];
		Text(Fnt_TimeBig, xw - 84, y-2);
	}
	
	//  volume  %
	Clr(100,140,220);
	str = i2s(audio->iVolume/10) + "%";
	Text(Fnt_Info, xw-40, y+25);
	
	
	///  debug
	///==================
	if (bDebug)
	{
		Clr(120,180,240);
		/*str = "s "+b2s(shift)+" c "+b2s(ctrl)+" a "+b2s(alt);
		Text(Fnt_Info, 50, 10);/**/
		str = "b "+i2s(yB_pl)+" e "+i2s(yE_pl)+" m "+i2s(ym)+" be "+i2s(yE_pl-yB_pl)+
			" yL " + i2s(yL_pl);/**/
		Text(Fnt_Info, 50, 30);
		str = "t "+i2s(pls->GetTracks().size())+" c "+i2s(pls->cur)+
			" o "+i2s(pls->ofs)+" oe "+i2s(pls->ofs+yL_pl);
		Text(Fnt_Info, 50, 46);/**/
	#if 0
		float xk1 = (view.xSize-view.xW_plS+1),	 xk2 = view.xSize - xk1;
		//float xp1 = (view.xSize-2*view.xW_plS/3+1),  xp2 = (view.xSize-view.xW_plS/2+1);
		str = f2s(xk1)+" "+i2s(yB_pl)+" "+f2s(xk2)+" "+i2s(yE_pl);
		Text(50,30);

		//str = "l " + i2s(bL?1:0)+" m "+i2s(bM?1:0)+" y "+i2s(ym)+" iv "+f2s(mtiv);
		//Text(50,50);
	#endif
	}
}

