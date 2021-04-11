#include "AppSFML_Draw.h"
using namespace std;  using namespace sf;


//  Pls Slider  |
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPls_Slider()
{
	const ViewSet& v = set.view;
	const int xw = v.wnd.xSize, xs = v.pls.xW_slider;
	if (yB_pl_inf >= v.wnd.ySize || xs <= 0)  return;

	const int len = Pls().LengthVis();  const float fle = len;
	const float yr = mia(1.f, 2.f, fle / yL_pl);  // row h pixels
	const int
		xk1 = xw - xs +1,
		xk2 = xw - xk1,
		xp1 = xw - 2*xs/3 +1,
		xp2 = xw - xs/2 +1;

	
	//  clear backgr  par-
	Rect(xp1-10, yB_pl, xk2+10, yE_pl-yB_pl, TX_Black, false);
	
	
	//  bookmarks  *
	const Uint8 b = 100;  //par
	for (int i=0; i < len; ++i)
	{
		const int bk = Pls().GetTrackVis(i).GetBookmark();
		if (bk > 0)
		{
			float fc1 = i /fle,  fc2 = (i + yr) /fle;			if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			int tex = TX_SliB1 + bk - 1;
			Rect(xp1, float(c1), xw - xp1, float(c2-c1), ETexUV(tex), true, b,b,b);
	}	}

	
	//  find results  -
	if (bFind)
	for (int i=0; i < len; ++i)
		if (Pls().GetTrackVis(i).IsFound())
		{
			float fc1 = i /fle,  fc2 = (i + yr) /fle;			if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			Rect(xk1, float(c1), xp2, float(c2-c1), TX_SliF);
		}
	
	
	//  slider bar  |  visble area
	if (len <= yL_pl/2)  return;

	float fc1 = Pls().iCur /fle,  fc2 = (Pls().iCur + 1) /fle;		if (fc2>1.f) fc2=1.f;  // cursor
	float fs1 = Pls().iOfs /fle,  fs2 = (Pls().iOfs +yL_pl) /fle;	if (fs2>1.f) fs2=1.f;  // vis list
	int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;  // min h = 2 pix _
	int s1 = fs1 *yH_pl+yB_pl, s2 = fs2 *yH_pl+yB_pl;  if (s2-s1<2)  s2=s1+2;

	const Uint8 c = 180;  //par
	if (len > yL_pl)
	Rect(xk1, s1, xk2, s2-s1, TX_Slider, true, c,c,c);
	Rect(xk1, c1, xk2, c2-c1, TX_SliCur, true);

	
	//  playing  _  cursor
	{
		float fc1 = Pls().iPlayVis /fle,  fc2 = /*fc1+4.f/yw*/(Pls().iPlayVis + 1.f) /fle;  if (fc2>1.f) fc2=1.f;
		int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;

		Rect(xk1, float(c1), xk2, float(c2-c1), TX_SliPlay);
	}
	//  selected-

	
	///  all tracks  rating
	if (v.pls.bSliderRate && !bFind)
		for (int i=0; i < len; ++i)
		{
			const int rate = Pls().GetTrackVis(i).GetRate(), r = rate + Ratings::cntMin;
			if (rate != 0)
			{
				float fc1 = float(i) /fle, fc2 = float(i + yr) /fle;  if (fc2>1.f) fc2=1.f;
				int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<1) c2=c1+1;
	
				Rect(xk1, float(c1), xk2, float(c2-c1+1),
					Ratings::GetTex(rate), true,
					//clrRateBck[r][0], clrRateBck[r][1], clrRateBck[r][2]);  //par
					clrRateBck[r][0]*2/3, clrRateBck[r][1]*2/3, clrRateBck[r][2]*2/3);  //par
		}	}
	
	//  todo: fill texture in Update() and just draw once
}
