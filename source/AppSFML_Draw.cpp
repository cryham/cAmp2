#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
using namespace std;  using namespace sf;


void AppSFMLDraw::DrawPlayer()
{
	//for (const auto& e:skin)
	//	e.Draw();
	
	int xw = set.xwSize, yw = set.ywSize;
	bool play = audio->IsPlaying();

	//  params
	int h = 150; //160;
	const int uy=40 /*>0 darker*/, uh=52 /*0 line..fire 512*/;

	//  backgr
	Rect(0,0, xw,h, TX_BackPlr);
	
	//  vis FFT  ~~~~~
	if (play)
	{
		audio->GetVisData(xw);
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
			Rect(i, 10 + h-y, 1,y, 475,uy,1,uh, r,g,b);
		}
	}/**/
	h += 10;

	//  Position bar  --=---
	//if (play)
	{
		audio->getPos();
		
		int yB_pos = h, yE_pos = 9;  float xW_pos = 0.03f;

		Rect(0.f,yB_pos, xw,yE_pos, TX_PosDk);

		if (play)
		{
		float xp = (audio->timePlay / audio->timeTrack) * (1.f-xW_pos), //marg,bar dim
			x1 = xp*xw;  int xb = xW_pos*xw;
		
		const auto& t = ciTexUV[TX_PosBr];
		Rect(x1, yB_pos,  xb, yE_pos, 
			 t.x+t.w*0.48f, t.y,  t.w*0.04f, t.h);
			 //TX_PosBr, 0.48f,0.52f); //xp,xp+xWpo);
	}	}

	if (bFps)
	{
		Clr(120,160,240);
		str = f2s(1.f / dt);
		Text(60,25);
	}
	h += 10;

	
	//  play icons  .
	int y = 2;
	Clr(50,120,120);
	str = audio->IsPaused() ? "||" : play ? "|>" : "[]";
	//str = audio->IsPaused() ? "\u23F8" : play ? "\u23F5" : "\u23F9";
	Text(xw/2,y);
	
	if (audio->bRep1)
	{
		Clr(100,100,50);
		str = "@1";
		Text(xw/2+50,y+25);
	}

	//  play info text  * * *
	if (play)
	{
		Clr(100,140,190);
		str = audio->sInfo;
		Text(10,y);
				
		Clr(150,190,240);
		bool below10 = audio->timeTrack < 10.0;
		str = t2s(audio->timePlay, below10);
		Text(xw-140,y);

		Clr(130,160,220);
		str = t2s(audio->timeTrack);
		Text(xw-60,y);

		Clr(120,160,240);
		str = i2s(audio->iVolume/10) + "%";
		Text(xw-40,y+25);
	}
	
	
	//  playlist  ==
	//-------------------------------
	const auto& tracks = pls->GetTracks();
	int yp = h, yF = 16,
		it = pls->ofs;
	
	//  visible lines  // dn_marg-
	yL_pl = (yw-yF*1 - yp)/yF;
	pls->lin = yL_pl;
	//  dbg
	str = "yl " + i2s(yL_pl)+" c "+i2s(pls->cur)+" o "+i2s(pls->ofs);
	Text(50,30);
	
	if (yL_pl > 0)
	if (tracks.empty())
	{
		Clr(120,160,240);
		str = "Playlist empty.";
		// Drop or Insert files..";
		Text(0,yp);
	}
	else
	for (int yi=0; yi < yL_pl; ++yi)
	if (it < tracks.size())
	{
		const Track& t = tracks[it];
		
		//  cursors
		if (it == pls->play)
		{	Uint8 c = play ? 255 : 128;
			Rect(0,yp,xw,20, 0,90,251,12, c,c,c);
		}
		if (it == pls->cur)
			Rect(0,yp,xw,20, 0,70,241,10, 255,255,255);
		// todo: blend add
		
		//  name
		const static sf::Uint8 c[chRall][3] = 
		{
			100,140,100, //-3
			110,160,110, //-2
			120,180,120, //-1
			 90,150,190, //0
			110,170,210, // 1
			120,180,220, // 2
			130,190,230, // 3
			140,200,240, // 4
			150,220,255, // 5
		};
		int r = t.rate+3;
		Clr(c[r][0],c[r][1],c[r][2]);
		str = t.GetName();
		Text(20,yp);

		str = chFRate[r];
		Text(3,yp);
		
		//  time
		if (t.GotTime())
		{
			Clr(180,200,240);
			str = t2s(t.GetTime());
			Text(xw-50,yp);
		}
		yp += yF;  ++it;
	}	
	
}
