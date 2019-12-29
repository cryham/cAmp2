#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <cmath>
using namespace std;  using namespace sf;


//  colors  // todo: in xml..
const static sf::Uint8 cc[chRall][3] = 
{
	100,140,100, //-3
	110,160,110, //-2
	120,180,120, //-1
	90,150,190, //0
	110,150,210, // 1
	120,170,220, // 2
	120,190,230, // 3
	140,200,240, // 4
	160,210,240, // 5
};
const static sf::Uint8 cb[chRall][3] = 
{
	00, 40, 00, //-3
	10, 60, 10, //-2
	20, 80, 20, //-1
	0, 50, 90, //0
	10, 50,110, // 1
	20, 70,120, // 2
	20, 90,130, // 3
	40,100,140, // 4
	60,110,140, // 5
};
static ETexUV Tex4Rate(int rate)
{
	return (ETexUV)mia(int(TX_Rate1),int(TX_Rate5), int(TX_Rate1-1+abs(rate)) );
}

void AppSFMLDraw::DrawPlayer()
{
	//for (const auto& e:skin)
	//	e.Draw();
	
	int xw = set.view.xSize, yw = set.view.ySize;
	bool play = audio->IsPlaying();
	
	//UpdDim();  // in resize, when needed

	//  params
	int h = yE_vis - yB_vis;
	const int uy=40 /*>0 darker*/, uh=52 /*0 line..fire 512*/;

	
	//  backgr  []
	Rect(0,0, xw,h, TX_BackPlr);
	
	
	//  Vis FFT  ~~~~~
	if (play)
	{
		audio->GetVisData(xw, set.view);
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
			Rect(i, yB_vis + h-y, 1,y, 475,uy,1,uh, r,g,b);
		}
	}/**/
	
	
	//  Position bar  --=---
	//if (play)
	{
		audio->getPos();
		
		int yH = yE_pos - yB_pos;
		Rect(0.f,yB_pos, xw,yH, TX_PosDk);

		if (play)
		{
			float xp = (audio->timePlay / audio->timeTrack) * (1.f-xW_pos), //marg,bar dim
				x1 = xp*xw;  int xb = xW_pos*xw;
			
			const auto& t = ciTexUV[TX_PosBr];
			Rect(x1, yB_pos,  xb, yH, 
				 t.x+t.w*0.48f, t.y,  t.w*0.04f, t.h);
				 //TX_PosBr, 0.48f,0.52f); //xp,xp+xWpo);
	}	}

	
	if (bFps)
	{
		Clr(120,160,240);
		str = f2s(1.f / dt);
		Text(Fnt_Info, 60, 16);
	}

	
	//  play icons  .
	int y = 2;
	Clr(50,90,120);
	//str = audio->IsPaused() ? "||" : play ? "|>" : "[]";  // simple
	str = audio->IsPaused() ? String(Uint32(0x25AE)) + String(Uint32(0x25AE))
		: play ? String(Uint32(0x25B6)) : String(Uint32(0x25A0));
	Text(Fnt_Info, xw/2, y);
	
	//  repeat  @
	if (audio->bRep1)
	{
		Clr(100,100,50);
		str = String(Uint32(0x21BB))+"1"; //"↻▶▶▮▮■"); //"@1";
		Text(Fnt_Info, xw/2+50, y+25);  // +Fy TimeBig..
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
		Text(Fnt_TimeBig, xw - w - 100, y-2);

		//  Time track
		Clr(100,140,210);
		s = t2s(audio->timeTrack);  str = s;
		l = s.length();  w = l * w0;
		Text(Fnt_TimeBig, xw - w - 20, y-2);
		
		//  track rating
		int rr = audio->rate, r = rr+3;
		str = chFRateVis[r];
		Text(Fnt_TimeBig, xw - 75, y-2);

		//  volume  %
		Clr(120,160,240);
		str = i2s(audio->iVolume/10) + "%";
		Text(Fnt_Info, xw-40, y+25);
	}
	
	
	//  playlist  ==
	//--------------------------------------------------------------
	const auto& tracks = pls->GetTracks();
	int yp = yB_pl, yF = set.view.Fy,
		it = pls->ofs;
	
	//  visible lines  // dn_marg-
	//yL_pl = yL_pl; //(yw-yF*1 - yp)/yF;
	pls->lin = yL_pl;
	
	
	///  debug
	///==================
	if (bDebug)
	{
		Clr(120,180,240);
		str = "b "+i2s(yB_pl)+" e "+i2s(yE_pl)+" m "+i2s(ym)+" be "+i2s(yE_pl-yB_pl)+
			" yL " + i2s(yL_pl);
		Text(Fnt_Info, 50, 30);
		str = "t "+i2s(pls->GetTracks().size())+" c "+i2s(pls->cur)+
			" o "+i2s(pls->ofs)+" oe "+i2s(pls->ofs+yL_pl);
		Text(Fnt_Info, 50, 46);
	#if 0
		float xk1 = (view.xSize-view.xW_plS+1),	 xk2 = view.xSize - xk1;
		//float xp1 = (view.xSize-2*view.xW_plS/3+1),  xp2 = (view.xSize-view.xW_plS/2+1);
		str = f2s(xk1)+" "+i2s(yB_pl)+" "+f2s(xk2)+" "+i2s(yE_pl);
		Text(50,30);

		//str = "l " + i2s(bL?1:0)+" m "+i2s(bM?1:0)+" y "+i2s(ym)+" iv "+f2s(mtiv);
		//Text(50,50);
	#endif
	}


	if (yL_pl > 0)
	if (tracks.empty())
	{
		Clr(120,160,240);
		str = "Playlist empty.";
		// Drop or Insert files..";
		Text(Fnt_Track, 0, yp);
	}
	else
	{
		///  backgr, names  1st
		yp = yB_pl;  it = pls->ofs;

		int xws = set.view.xSize - set.view.xW_plS;

		for (int yi=0; yi < yL_pl; ++yi)
		if (it < tracks.size())
		{
			const Track& trk = tracks[it];
			
			//  rating backgr
			int rr = trk.rate, r = rr+3;
			Rect(0,yp,xw,yF,
				Tex4Rate(rr),
				cb[r][0], cb[r][1], cb[r][2]);

			//  cursors
			if (it == pls->play)
			{	Uint8 c = play ? 255 : 128;
				Rect(0,yp,xws,yF, 0,90,251,12, c,c,c);
			}
			if (it == pls->cur)
				Rect(0,yp,xws,yF, 0,70,241,10, 255,255,255);
			// todo: blend add
			
			//  rate
			str = chFRateVis[r];
			int w = Text(Fnt_Track, 0,0, false);  // center
			Text(Fnt_Track, max(0, 5 - w/2), yp);

			//  name
			Clr(cc[r][0],cc[r][1],cc[r][2]);
			str = trk.GetName();
			Text(Fnt_Track, 17, yp);
	
			yp += yF;  ++it;
		}
		
		//  const width
		//str = t2s(600.f);  // get for 10:00
		str = "0";
		const int w0 = Text(Fnt_Time, 0,0, false);
		const int ws = set.view.xW_plS + 8;  //par w time|slider
		
		//  times backgr clear text
		//  todo: per line..
		float xk1 = set.view.xSize - ws - 4  //par w track|time
			- 4 * w0,  xk2 = set.view.xSize - xk1;
		Rect(xk1, yB_pl, xk2, yE_pl-yB_pl, TX_Black);
		
		DrawSlider();

		///  times  2nd
		yp = yB_pl;  it = pls->ofs;

		for (int yi=0; yi < yL_pl; ++yi)
		if (it < tracks.size())
		{
			const Track& trk = tracks[it];

			//  time
			if (trk.GotTime())
			{
				float t = trk.GetTime();
				if (iTimeTest > 0)  // time colors test
					t = iTimeTest == 1 ?
						pow(float(it)/60.f, 1.2f) * 800.f :
						pow(float(it)/40.f, 1.2f) * 3000.f;
				
				TimeClr c = timeColors.Get(t);
				Clr(c.r*255.f, c.g*255.f, c.b*255.f);
	
				string s = t2s(t);
				int l = s.length();
				int w = l * w0;
				//int w = Text(Fnt_Time, 0, 0, false);
				str = s;
				Text(Fnt_Time, xw - ws /*- wm */- w/**/, yp);  // align right
			}
			yp += yF;  ++it;
		}		
	}
}


//  Slider
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawSlider()
{
	if (!pls)  return;
	if (yB_pl_inf >= set.view.ySize || set.view.xW_plS <= 0)  return;

	float fle = pls->Length();
	float ySr = mia(1.f, 2.f, fle / yL_pl);
	int xk1 = (set.view.xSize - set.view.xW_plS +1),
		xk2 = set.view.xSize - xk1;
	//int xp1 = (view.xSize - 2*view.xW_plS/3 +1), xp2 = (view.xSize-view.xW_plS/2+1);

	// rating ->
	#if 1  //..
		//if (view.bSlDrawR && !bShowSrch)
		const auto& trks = pls->GetTracks();
		for (int i=0; i < pls->Length(); i++)
		{
			const int rr = trks[i].rate, r = rr+3;
			if (rr != 0)
			{
			//DWORD rgb = rr > 0 ? 0xFF40A0FF : 0xFF80FF00;

			float fc1 = float(i) /fle, fc2 = float(i + ySr) /fle;  if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<1) c2=c1+1;

			Rect(xk1, float(c1), xk2, float(c2-c1+1),
				Tex4Rate(rr),
				cb[r][0], cb[r][1], cb[r][2]);
				//mia(int(TX_Rate1),int(TX_Rate5), int(TX_Rate1-1+abs(rr)) ),
				/*rr > 0 ? 0x40 : 0x80,
				rr > 0 ? 0xA0 : 0xFF,
				rr > 0 ? 0xFF : 0x00);*/
				//rgb);//, 0.15f, 0.25f);
		}  }
	#endif
	
	// search results +
		//ySr = mia(1.f, 1.f, float(pls->Length()) / yLpl);
	#if 0
		if (bShowSrch /*&& ySr > 0.8f*/)
		for (int i=0; i < pls->Length(); ++i)
		if (pls->vList[i]->srch > 0)
		{
			float fc1 = i /fle,  fc2 = (i + ySr) /fle;		if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			Rtex(TX_SliF, xk1, float(c1), xp2, float(c2));
		}

	// bookmarks <*
		for (int i=0; i < pls->Length(); ++i)
		if (pls->vList[i]->bokm > 0)
		{
			float fc1 = i /fle,  fc2 = (i + ySr) /fle;		if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			int tex = pls->vList[i]->bokm-1 + TX_SliB1;
			Rtex(tex, xp1, float(c1), xk2, float(c2));
		}
	#endif

	/* Pls slider | */
		if (pls->Length() <= yL_pl/2)  return;

		float fc1 = pls->cur /fle,  fc2 = (pls->cur + 1) /fle;	if (fc2>1.f) fc2=1.f;  // cursor
		float fs1 = pls->ofs /fle,  fs2 = (pls->ofs +yL_pl) /fle;	if (fs2>1.f) fs2=1.f;  // vis list
		int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;  // min h = 2 pix _
		int s1 = fs1 *yH_pl+yB_pl, s2 = fs2 *yH_pl+yB_pl;  if (s2-s1<2)  s2=s1+2;

		//D3DRECT rAll= {view.xSize-view.xW_plS+1, yB_pl, view.xSize, yE_pl};
		//pDev->Clear(1, &rAll, clFl, 0, 1.f, 0);
		
		if (pls->Length() > yL_pl)
		Rect(xk1, s1, xk2, s2-s1, TX_Slid);
		Rect(xk1, c1, xk2, c2-c1, TX_SliC);
		
	// playing _
	#if 0
		{
			float fc1 = pls->idPl /fle,  fc2 = /*fc1+4.f/yw*/(pls->idPl + 1.f) /fle;  if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;

			Rtex(TX_SliP, xk1, float(c1), xk2, float(c2));
		}
	#endif
	// selected-

}
