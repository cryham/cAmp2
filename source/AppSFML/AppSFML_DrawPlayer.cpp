#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include <cmath>
#include <cstring>
using namespace std;  using namespace sf;


//  Draw all
//----------------------------------------------------
void AppSFMLDraw::DrawAll()
{
	if (Pls().bDraw)
	{	Pls().bDraw = false;
		iDraw = 4;  //par fixes double buffer flicker?
	}
	if (iDraw > 0)  // clear backgr
		pWindow->clear();
	
	DrawPlayer();   // always
	
	if (iDraw > 0)
	{	--iDraw;
		
		DrawTabs();  // after change
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
	const int xw = v.wnd.xSize, yw = v.wnd.ySize, xhalf = xw/2, y0 = 2;
	const bool play = audio->IsPlaying();
	const int yl2 = v.FontH(Fnt_TimeBig) + 5;  // below times
	const int yl1 = v.FontH(Fnt_Player) + 5;  // below info

	
	//  backgr  []  clear
	Rect(0,0, xw,yE_vis, TX_BackPlr, false);
	
	DrawVisual();
	
	
	//  Position bar  --=---
	int yH = yE_pos - yB_pos;
	Rect(0.f,yB_pos, xw,yH, TX_PosDk, false);

	if (play)
	{
		audio->GetPos();
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
		Text(Fnt_Player, 60, 16);
	}


	str = "100%";
	const int wDigit = Text(Fnt_Player, 0,0, false) / 4;
	
	
	//  Prev, Next  |< >|  hover
	if (ym > 0 && ym < yE_plr_btn)  // prev,next  btns |< >|
	{
		if (xm < xhalf)  Clr(90,150,210);  else  Clr(50,90,130);
		str = String(Uint32(0x25AE)) + String(Uint32(0x25C0));
		Text(Fnt_Player, xhalf - 3*wDigit, yl2);

		if (xm > xhalf)  Clr(90,150,210);  else  Clr(50,90,130);
		str = String(Uint32(0x25B6)) + String(Uint32(0x25AE));
		Text(Fnt_Player, xhalf + 2*wDigit, yl2);
	}
	
	//  play icons  |>
	Clr(50,90,120);
	//str = audio->IsPaused() ? "||" : play ? "|>" : "[]";  // simple
	str = audio->IsPaused() ? String(Uint32(0x25AE)) + String(Uint32(0x25AE))
		: play ? String(Uint32(0x25B6)) : String(Uint32(0x25A0));  // ▮▮ ▶ ■
	Text(Fnt_Player, xw - 12*wDigit, yl2);

	//  Repeat  @
	if (audio->bRepTrk || audio->bRepPls)
	{
		Clr(50,100,100);
		str = String(Uint32(0x21BB));  // ↻
		if (audio->bRepTrk)  str += "1";
		//else  str += "A";
		Text(Fnt_Player, xw - 9*wDigit, yl2+1);
	}
	
	//  Volume  %
	Clr(100,140,220);
	str = i2s(audio->iVolume/10) + "%";
	int w = Text(Fnt_Player, 0,0, false);
	Text(Fnt_Player, xw - w - 1*wDigit, yl2+1);

	

	//  player  info Text  * * *
	if (play)
	{
		//  ext bitrate freq size
		Clr(110,140,190);
		str = audio->sInfo;
		Text(Fnt_Player, 10, y0);
				
		//  time right align..
		str = "0:12:34";
		const int wDigit = Text(Fnt_TimeBig, 0,0, false) / 7;
		string s;  int len, w, x;

		bool below10s = audio->timeTrack < 10.0;
		bool over1Min = audio->timeTrack >= 60.0;
		//  Time track
		Clr(100,140,210);
		s = t2s(audio->timeTrack, below10s);  str = s;
		len = s.length() +1;  w = len * wDigit;
		Text(Fnt_TimeBig, xw - w, 0);  x = w;

		//  track rating
		Clr(120,160,220);
		str = Ratings::GetVis(audio->rate);
		x += 3 * wDigit;
		Text(Fnt_TimeBig, xw - x, 0);
		x += 1 * wDigit;

		//  Time cur
		Clr(120,160,220);
		s = t2s(audio->timePlay, below10s, over1Min);  str = s;
		len = s.length() +1;  w = len * wDigit;
		Text(Fnt_TimeBig, xw - w - x, 0);
	}

	//  File info  ----
	if (set.bFileInfo)
	{
		const Track& trk = Pls().GetTrackVis(Pls().iCur);
		string s = trk.GetPath();
		
		auto f = s.find('/', 12);  // add \n in 3rd /
		if (f != string::npos)
			s.insert(s.begin() + f, '\n');
		f = s.find_last_of('/');  // add \n in last /
		if (f != string::npos)
			s.insert(s.begin() + f, '\n');

		str = Str(s);
		Clr(120,180,240);
		Text(Fnt_Player, 10, 46);  // todo: wrap text..?
	}

	
	//  Osd  ----
	if (dtOsd > 0.f)
	{
		dtOsd -= dt;
		str = osdStr;
		float f = 0.7f * dtOsd / dtOsdShow + 0.3f;
		Clr(160*f,250*f,250*f);
		Text(Fnt_Player, 10, yl2);
	}
	else
	//  Find  ----
	if (bFind && iFoundAll > 0)
	{
		str = "Found: See "+i2s(iFoundSee)+" Vis "+i2s(Pls().GetFoundVis())+
			+" / Pls "+i2s(Pls().GetFound())+"  All "+i2s(iFoundAll);
		Clr(100,220,100);  // center par
		Text(Fnt_Player, 10 /*(xw - 140) / 2*/, yl2);
	}

	
	///  Debug  ~~~~
	if (bDebug && !Pls().IsEmpty())
	{
		Clr(120,180,240);
		int ymc = (ym - yB_pl) / v.FontH(Fnt_Tracks);  ymc = max(0, min(ymc, yL_pl-1));
		str = "curId "+i2s(Pls().GetTrackVisIdAll(Pls().iCur))
			+" cur-ofs "+i2s(Pls().iCur-Pls().iOfs)+" len "+i2s(Pls().LengthVis());
		Text(Fnt_Player, 50, 30);
		str = "mId "+i2s(Pls().GetTrackVisIdAll( min(Pls().iOfs+ymc, Pls().LengthVis()-1) ))
			+" ym "+i2s(ymc)+" / yL "+i2s(yL_pl);
		Text(Fnt_Player, 50, 46);
		str = "pl "+i2s(Pls().iPlay)+" plVis "+i2s(Pls().iPlayVis)+" "+b2s(Pls().bPlayVisOut);
		Text(Fnt_Player, 50, 62);
	}
}
