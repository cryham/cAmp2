#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <cmath>
using namespace std;  using namespace sf;


//  colors  // todo: in xml..
const Uint8 AppSFMLDraw::clrRateTxt[chRall][3] = 
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
const Uint8 AppSFMLDraw::clrRateBck[chRall][3] = 
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
	int len = Pls().LengthVis();
	bool play = audio->IsPlaying();
	const ViewSet& v = set.view;
	int xw = v.wnd.xSize;
	int yp = yB_pl, yF = v.fnt.Fy,
		it = Pls().iOfs;
	
	//  visible lines  // dn_marg-
	Pls().SetVisLines(yL_pl);


	if (yL_pl <= 0)  return;
	if (Pls().IsEmpty())
	{
		Clr(120,160,240);
		str = "Playlist empty.";
		// Drop or Insert files..";
		Text(Fnt_Track, 0, yp);
		return;
	}

	///  backgr, Names  1st pass
	yp = yB_pl;  it = Pls().iOfs;

	int xws = v.wnd.xSize - v.pls.xW_slider;

	int iFindVis = 0;
	for (int yi=0; yi < yL_pl; ++yi)
	if (it < len)
	{
		const Track& trk = Pls().GetTrackVis(it);
		bool dir = trk.IsDir();
		
		//  rating backgr
		int rr = trk.rate, r = rr + cRmin;
		if (!dir && rr)
		Rect(0,yp,xw,yF,
			Tex4Rate(rr), false,
			clrRateBck[r][0], clrRateBck[r][1], clrRateBck[r][2]);

		//  rate
		Clr(clrRateTxt[r][0], clrRateTxt[r][1], clrRateTxt[r][2]);
		str = GetRateStr(rr);
		int w = Text(Fnt_Track, 0,0, false);  // center
		Text(Fnt_Track, max(0, 5 - w/2), yp);

		//  dir / path
		if (dir)
		{	Clr(140,140,200);
			switch (set.eDirView)
			{
			case DirV_Fullpath:  str = Str(trk.GetPath());  break;
			case DirV_Path:  str = Str(trk.GetName());  break;
			case DirV_Path2:  str = Str(trk.GetName()) + " / " + Str(trk.GetParent());  break;
			case DirV_Path3:  str = Str(trk.GetName()) + " / " + Str(trk.GetParent()) + " / " + Str(trk.GetParent2());  break;
			default:  str = "";
			}
		}else  //  name
		{	if (trk.IsDisabled())
				Clr(50,80,100);
			str = String::fromUtf8(trk.GetName().begin(), trk.GetName().end());
		}
		if (trk.found)  // find
		{
			++iFindVis;
			if (bFind)
				Clr(70,240,70);
		}
		Text(Fnt_Track, 17, yp);

		yp += yF;  ++it;
	}
	iFoundVis = iFindVis;
	
	
	//  const width
	//str = t2s(600.f);  // get for 10:00
	str = "0";
	const int w0 = Text(Fnt_Time, 0,0, false);
	const int ws = v.pls.xW_slider + 8;  //par w time|slider
	
	//  times backgr clear text
	//  todo: per line..
	float xk1 = v.wnd.xSize - ws - 4  //par w track|time
		- 4 * w0,  xk2 = v.wnd.xSize - xk1;
	Rect(xk1, yB_pl, xk2, yE_pl-yB_pl, TX_Black, false);
	
	DrawPlsSlider();

	///  Times  2nd pass
	yp = yB_pl;  it = Pls().iOfs;

	for (int yi=0; yi < yL_pl; ++yi)
	if (it < len)
	{
		const Track& trk = Pls().GetTrackVis(it);

		//  time
		if (trk.HasTime() && !trk.IsDisabled())
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
			str = s;  // align right
			Text(Fnt_Time, xw - ws /*- wm */- w/**/, yp);
		}
		yp += yF;  ++it;
	}		

	DrawPlaylist_3Cursors();

	DrawPlsHeader();
}


///  cursors  3rd pass
void AppSFMLDraw::DrawPlaylist_3Cursors()
{
	const bool play = audio->IsPlaying();
	const ViewSet& v = set.view;
	const int xws = v.wnd.xSize - v.pls.xW_slider;
	const int len = Pls().LengthVis(), yF = v.fnt.Fy;

	int yp = yB_pl,
		it = Pls().iOfs;
	
	for (int yi=0; yi < yL_pl; ++yi)
	if (it < len)
	{
		const Track& trk = Pls().GetTrackVis(it);
	
		//  bookmarks
		const Uint8 b = 150;  //par
		if (trk.bookm > 0)
			Rect(0,yp,xws,yF,
				ETexUV(TX_PlsB1 + trk.bookm - 1), true, b,b,b);
		
		//  cursors
		if (it == Pls().iPlayVis)
		{	bool sh = Pls().bPlayVisOut;  // out shorter
			const auto& t = ciTexUV[TX_PlsPlay];
			const Uint8 c = play ? 195 : 138;  //par
			RectUV(0,yp, xws, sh ? yF/3 : yF,  t.x,t.y, t.w,sh ? t.h/3 : t.h,  true, c,c,c);
		}
		const Uint8 c = 255;  //par dim
		if (it == Pls().iCur)
			Rect(0,yp,xws,yF, TX_PlsCur, true, c,c,c);
	
		yp += yF;  ++it;
	}
}
