#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../System/defines.h"
#include <cmath>
using namespace std;  using namespace sf;


//  colors  // todo: in xml..
const Uint8 AppSFMLDraw::clrRateTxt[Ratings::cntAll][3] = 
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
const Uint8 AppSFMLDraw::clrRateBck[Ratings::cntAll][3] = 
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


//  Playlist  ==
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPlaylist()
{
	//  visible lines
	Pls().SetVisLines(yL_pl);
	if (yL_pl <= 0)  return;

	if (Pls().IsEmpty())
	{
		Clr(120,160,240);
		str = "Playlist empty.";
		Text(Fnt_Track, 0, yB_pl);
		//str = "Drop or Insert files..";
		//Text(Fnt_Track, 0, yB_pl);
		return;
	}

	//  Draw
	DrawPls_1Names();
	DrawPls_2Times();
	DrawPls_3Cursors();

	DrawPls_Header();
	DrawPls_Slider();
	// todo: down marg_
}


///  backgr, Names  1st pass
void AppSFMLDraw::DrawPls_1Names()
{
	const ViewSet& v = set.view;
	//const int xws = v.wnd.xSize - v.pls.xW_slider;
	const int xw = v.wnd.xSize;
	const int len = Pls().LengthVis(), yF = v.fnt.Fy;
	
	int iFindVis = 0;
	plsTxtW.clear();
	plsTxtW.reserve(yL_pl);

	
	int y = yB_pl, it = Pls().iOfs;
	for (int yl=0; yl < yL_pl; ++yl)
	if (it < len)
	{
		const Track& trk = Pls().GetTrackVis(it);
		const bool dir = trk.IsDir();
		
		//  rating backgr
		const int rate = trk.GetRate(), r = rate + Ratings::cntMin;
		if (!dir && rate)
		Rect(0,y, xw,yF,
			Ratings::GetTex(rate), false,
			clrRateBck[r][0], clrRateBck[r][1], clrRateBck[r][2]);
	
		//  rate
		str = Ratings::GetVis(rate);
		int w = Text(Fnt_Track, 0,0, false);  // center
		Text(Fnt_Track, max(0, 5 - w/2), y);

		//  hide  clr
		const auto h = trk.GetHide();
		int c = 0;
		if (h == Hid_Hide)		c = -30;
		else if (h == Hid_Show)	c = +30;
	
		//  name  ----
		if (dir)  //  dir / path
		{
			Clr(140+c,140+c,200+c);

			switch (set.eDirView)
			{
			case DirV_Fullpath:  str = Str(trk.GetPath());  break;
			case DirV_Path:  str = Str(trk.GetName());  break;
			case DirV_Path2:  str = Str(trk.GetName()) + " / " + Str(trk.GetParent());  break;
			case DirV_Path3:  str = Str(trk.GetName()) + " / " + Str(trk.GetParent()) + " / " + Str(trk.GetParent2());  break;
			default:  str = "";
			}
		}else  //  file
		{
			if (trk.IsDisabled())
				Clr(50,80,100);
			else if (c)
				Clr(140+c,160+c,200+c);
			else	// todo: hsv?
				Clr(clrRateTxt[r][0], clrRateTxt[r][1], clrRateTxt[r][2]);
				//Clr(clrRateTxt[r][0]+c, clrRateTxt[r][1]+c/2, clrRateTxt[r][2]);
			
			str = String::fromUtf8(trk.GetName().begin(), trk.GetName().end());
		}
		
		//  find
		if (trk.IsFound())
		{
			++iFindVis;
			if (bFind)
				Clr(70,240,70);
		}
		
		w = Text(Fnt_Track, 17, y);
		plsTxtW.emplace_back(w + 17);
	
		y += yF;  ++it;
	}
	iFoundVis = iFindVis;
}


///  Times  2nd pass
void AppSFMLDraw::DrawPls_2Times()
{
	const ViewSet& v = set.view;
	const int xw = v.wnd.xSize;
	const int ws = v.pls.xW_slider + 8;  //par w  time|slider
	const int len = Pls().LengthVis(), yF = v.fnt.Fy;
	
	str = "0";  //  digit width
	const int w0 = Text(Fnt_Time, 0, 0, false);
	const int mt = 10;  // marg time|
	const int mh = yF+2;  // + - size
	
	
	int y = yB_pl, it = Pls().iOfs;
	for (int yl=0; yl < yL_pl; ++yl)
	if (it < len)
	{
		const Track& trk = Pls().GetTrackVis(it);
		const bool time = trk.HasTime() && !trk.IsDisabled();
		int w = 0;  float t = 0.f;
	
		if (time)
		{
			//  time
			t = trk.GetTime();
			if (iTimeTest > 0)  // time colors test
				t = iTimeTest == 1 ?
					pow(float(it)/60.f, 1.2f) * 800.f :
					pow(float(it)/40.f, 1.2f) * 3000.f;
			
			//  get width
			string s = t2s(t);
			str = s;
			//int w = Text(Fnt_Time, 0, 0, false);
			w = s.length() * w0;
		}
		
		//  backgr  to clear names text
		int xt = xw - ws - w;
		Rect(xt -mt, y, xw - xt +mt, yF, TX_Black, false);

		//  hide icons  + -
		auto h = trk.GetHide();
		if (h == Hid_Hide)		Rect(xt-mh+4,y+1, mh,mh, TX_DHide, true);
		else if (h == Hid_Show)	Rect(xt-mh+5,y+1, mh,mh, TX_DShow, true);
		
		if (time)
		{
			//  text
			TimeClr c = timeColors.Get(t);
			Clr(c.r*255.f, c.g*255.f, c.b*255.f);
	
			Text(Fnt_Time, xt, y);  // align right
			
			//  more signs, Text ..Time
			int vw = plsTxtW[yl];
			if (vw > xt -mt)
			{
				str = "..";
				Clr(170,200,230);
				Text(Fnt_Track, xt -mt, y+1);
			}
		}
		y += yF;  ++it;
	}		
}


///  Cursors  3rd pass
void AppSFMLDraw::DrawPls_3Cursors()
{
	const bool play = audio->IsPlaying();
	const ViewSet& v = set.view;
	const int xws = v.wnd.xSize - v.pls.xW_slider;
	const int len = Pls().LengthVis(), yF = v.fnt.Fy;

	int y = yB_pl, it = Pls().iOfs;
	for (int yl=0; yl < yL_pl; ++yl)
	if (it < len)
	{
		const Track& trk = Pls().GetTrackVis(it);
	
		//  bookmarks
		const Uint8 b = 150;  //par
		const auto bk = trk.GetBookmark();
		if (bk > 0)
			Rect(0,y, xws,yF,
				ETexUV(TX_PlsB1 + bk - 1), true, b,b,b);
		
		//  cursors
		if (it == Pls().iPlayVis)
		{
			bool sh = Pls().bPlayVisOut;  // out shorter

			const auto& t = ciTexUV[TX_PlsPlay];
			const Uint8 c = play ? 195 : 138;  //par
			
			RectUV(0,y, xws, sh ? yF/3 : yF,
				t.x,t.y, t.w,sh ? t.h/3 : t.h,  true, c,c,c);
		}
		const Uint8 c = 255;  //par dim
		if (it == Pls().iCur)
			Rect(0,y,xws,yF, TX_PlsCur, true, c,c,c);
	
		y += yF;  ++it;
	}
}