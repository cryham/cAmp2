#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <cmath>
#include <cstring>
using namespace std;  using namespace sf;


//  Draw all
//----------------------------------------------------
void AppSFMLDraw::DrawAll()
{
	if (Pls().bDraw)
	{	Pls().bDraw = false;
		iDraw = 2;
	}
	if (iDraw > 0)
		pWindow->clear();
	
	DrawPlayer();
	
	if (iDraw > 0)
	{	--iDraw;
		
		DrawTabs();
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
	const int xw = v.wnd.xSize, yw = v.wnd.ySize;
	const bool play = audio->IsPlaying();

	
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
		Text(Fnt_Info, 60, 16);
	}

	
	//  Prev, Next  |< >|  hover
	int y = 2;
	if (ym > 0 && ym < yE_plr_btn)  // prev,next  btns |< >|
	{
		if (xm < xw/2)  Clr(90,150,210);  else  Clr(50,90,130);
		str = String(Uint32(0x25AE)) + String(Uint32(0x25C0));
		Text(Fnt_Info, xw/2 - 30, y);

		if (xm > xw/2)  Clr(90,150,210);  else  Clr(50,90,130);
		str = String(Uint32(0x25B6)) + String(Uint32(0x25AE));
		Text(Fnt_Info, xw/2 + 10, y);
	}else
	{
		//  play icons  |>
		y = 2;
		Clr(50,90,120);
		//str = audio->IsPaused() ? "||" : play ? "|>" : "[]";  // simple
		str = audio->IsPaused() ? String(Uint32(0x25AE)) + String(Uint32(0x25AE))
			: play ? String(Uint32(0x25B6)) : String(Uint32(0x25A0));  // ▮▮ ▶ ■
		Text(Fnt_Info, xw/2, y);
	}
	
	
	//  Repeat  @
	if (audio->bRepTrk || audio->bRepPls)
	{
		Clr(50,100,100);
		str = String(Uint32(0x21BB));  // ↻
		if (audio->bRepTrk)  str += "1";
		//else  str += "A";
		Text(Fnt_Info, xw-75, y+25);  // +Fy TimeBig..
	}
	
	//  Volume  %
	Clr(100,140,220);
	str = i2s(audio->iVolume/10) + "%";
	Text(Fnt_Info, xw-40, y+25);
	

	//  player  info Text  * * *
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
		Text(Fnt_Info, 10, 46);  // todo: wrap text..?
	}
	
	//  Osd  ----
	if (dtOsd > 0.f)
	{
		dtOsd -= dt;
		str = osdStr;
		float f = 0.7f * dtOsd / dtOsdShow + 0.3f;
		Clr(160*f,250*f,250*f);
		Text(Fnt_Info, 10, v.vis.eType == VisT_None ? 22 : 32);
	}
	else
	//  Find  ----
	if (bFind && iFoundAll > 0)
	{
		str = "Found: "+i2s(iFoundVis)+" of "+i2s(Pls().GetFound())+"  All "+i2s(iFoundAll);
		Clr(100,220,100);  // center par
		Text(Fnt_Info, (xw - 140) / 2, v.vis.eType == VisT_None ? 22 : 32);
	}
	
	///  Debug  ~~~~
	if (bDebug)
	{
		Clr(120,180,240);
		int ymc = (ym - yB_pl) / v.fnt.Fy;  ymc = max(0, min(ymc, yL_pl-1));
		str = "curId "+i2s(Pls().GetTrackVisIdAll(Pls().iCur))
			+" cur-ofs "+i2s(Pls().iCur-Pls().iOfs)+" len "+i2s(Pls().LengthVis());
		Text(Fnt_Info, 50, 30);
		str = "mId "+i2s(Pls().GetTrackVisIdAll( min(Pls().iOfs+ymc, Pls().LengthVis()-1) ))
			+" ym "+i2s(ymc)+" / yL " + i2s(yL_pl);
		Text(Fnt_Info, 50, 46);
		str = "pl "+i2s(Pls().iPlay)+" plVis "+i2s(Pls().iPlayVis)+" " + b2s(Pls().bPlayVisOut);
		Text(Fnt_Info, 50, 62);
	}
}
