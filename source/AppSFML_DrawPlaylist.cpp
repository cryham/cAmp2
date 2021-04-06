#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <cmath>
using namespace std;  using namespace sf;


//  colors  // todo: in xml..
const static Uint8 cc[chRall][3] = 
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
const static Uint8 cb[chRall][3] = 
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
	return (ETexUV)mia(int(TX_Rate1), int(TX_Rate5),
					   int(TX_Rate1 - 1 + abs(rate)) );
}


//  Playlist  ==
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPlaylist()
{
	//const auto& tracks = Pls().GetTracks();
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
			cb[r][0], cb[r][1], cb[r][2]);

		//  rate
		Clr(cc[r][0],cc[r][1],cc[r][2]);
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

	///  cursors  3rd pass
	yp = yB_pl;  it = Pls().iOfs;

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
	
	DrawPlsHeader();
}


//  Pls Slider  |
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPlsSlider()
{
	const ViewSet& v = set.view;
	const int xw = v.wnd.xSize, xs = v.pls.xW_slider;
	if (yB_pl_inf >= v.wnd.ySize || xs <= 0)  return;

	int len = Pls().LengthVis();  float fle = len;
	float ySr = mia(1.f, 2.f, fle / yL_pl);
	int xk1 = xw - xs +1,
		xk2 = xw - xk1;
	int xp1 = xw - 2*xs/3 +1,
		xp2 = xw - xs/2 +1;

	//  find results  -
	if (bFind)
	for (int i=0; i < len; ++i)
		if (Pls().GetTrackVis(i).found)
		{
			float fc1 = i /fle,  fc2 = (i + ySr) /fle;		if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			Rect(xk1, float(c1), xp2, float(c2-c1), TX_SliF);
		}

	//  bookmarks  *
	const Uint8 b = 100;  //par
	for (int i=0; i < len; ++i)
	{
		const int bk = Pls().GetTrackVis(i).bookm;
		if (bk > 0)
		{
			float fc1 = i /fle,  fc2 = (i + ySr) /fle;		if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			int tex = TX_SliB1 + bk - 1;
			Rect(xp1, float(c1), xw - xp1, float(c2-c1), ETexUV(tex), true, b,b,b);
	}	}

	//  slider  |
	if (len <= yL_pl/2)  return;

	float fc1 = Pls().iCur /fle,  fc2 = (Pls().iCur + 1) /fle;	if (fc2>1.f) fc2=1.f;  // cursor
	float fs1 = Pls().iOfs /fle,  fs2 = (Pls().iOfs +yL_pl) /fle;	if (fs2>1.f) fs2=1.f;  // vis list
	int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;  // min h = 2 pix _
	int s1 = fs1 *yH_pl+yB_pl, s2 = fs2 *yH_pl+yB_pl;  if (s2-s1<2)  s2=s1+2;

	const Uint8 c = 180;  //par
	if (len > yL_pl)
	Rect(xk1, s1, xk2, s2-s1, TX_Slider, true, c,c,c);
	Rect(xk1, c1, xk2, c2-c1, TX_SliCur, true);
		
	//  playing _
	{
		float fc1 = Pls().iPlayVis /fle,  fc2 = /*fc1+4.f/yw*/(Pls().iPlayVis + 1.f) /fle;  if (fc2>1.f) fc2=1.f;
		int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;

		Rect(xk1, float(c1), xk2, float(c2-c1), TX_SliPlay);
	}
	//  selected-

	
	///  all tracks rating ->
	//  todo: fill texture in Update() and just draw once
	if (v.pls.bSliderRate && !bFind)
		for (int i=0; i < len; ++i)
		{
			const int rr = Pls().GetTrackVis(i).rate, r = rr+3.;
			if (rr != 0)
			{
				float fc1 = float(i) /fle, fc2 = float(i + ySr) /fle;  if (fc2>1.f) fc2=1.f;
				int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<1) c2=c1+1;
	
				Rect(xk1, float(c1), xk2, float(c2-c1+1),
					Tex4Rate(rr), true,
					cb[r][0]*2/3, cb[r][1]*2/3, cb[r][2]*2/3);  //par
		}	}
}
