#include "AppSFML_Draw.h"
using namespace std;  using namespace sf;


//  Pls Slider  |
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPls_Slider()
{
	const ViewSet& v = set.view;
	const int xw = v.wnd.xSize, xs = v.sldr.width;
	if (yB_pl_inf >= v.wnd.ySize || xs <= 0)  return;

	const int len = Pls().LengthVis();
	const float fLen = len,
		yr = mia(1.f, 2.f, fLen / yL_pl);  // row h pixels
	const int
		xk1 = xw - xs +1,
		xk2 = xw - xk1,
		xp1 = xw - 2*xs/3 +1,
		xp2 = xw - xs/2 +1;

	//  var
	float fc1, fc2;
	int c1, c2;
	auto GetCur = [&](float i1, float i2, int m)
	{
		fc1 = i1 /fLen;
		fc2 = i2 /fLen;
		if (fc2 > 1.f)  fc2 = 1.f;

		c1 = fc1 *yH_pl +yB_pl;
		c2 = fc2 *yH_pl +yB_pl;
		if (c2-c1 < m)  c2 = c1 + m;
	};
	

	//  clear backgr  par-
	Rect(xw-xs, yB_pl, xs, yH_pl, TX_Black, false);
	
	
	//  bookmarks  *
	const Uint8 b = 100;  //par
	for (int i=0; i < len; ++i)
	{
		const int bk = Pls().GetTrackVis(i).GetBookmark();
		if (bk > 0)
		{
			const int tex = TX_SliB1 + bk - 1;
			GetCur(i, i + yr, 1);
			Rect(xp1, float(c1), xw - xp1, float(c2-c1), ETexUV(tex), true, b,b,b);
	}	}

	
	//  find results  ==
	if (bFind)
	for (int i=0; i < len; ++i)
	{
		const auto& t = Pls().GetTrackVis(i);
		if (t.IsFound())
		{
			GetCur(i, i + yr, 1);
			Rect(xk1, float(c1), xp2, float(c2-c1), TX_SliF);
		}
		if (t.Btw().found)
		{
			GetCur(i, i + yr, 1);
			Rect(xk1, float(c1), xp2, float(c2-c1)*0.4f, TX_SliF);
		}
	}
	
	
	//  slider bar  |  visble area
	if (len <= yL_pl/2)
		return;

	GetCur(Pls().iOfs, Pls().iOfs +yL_pl, 2);  // vis list
	int s1 = c1, s2 = c2;
	GetCur(Pls().iCur, Pls().iCur + 1, 2);  // cursor

	const Uint8 c = 180;  //par
	if (len > yL_pl)
	Rect(xk1, s1, xk2, s2-s1, TX_Slider, true, c,c,c);
	Rect(xk1, c1, xk2, c2-c1, TX_SliCur, true);

	
	//  playing  _  cursor
	{
		GetCur(Pls().iPlayVis, Pls().iPlayVis + 1, 2);
		Rect(xk1, float(c1), xk2, float(c2-c1), TX_SliPlay);
	}


	//  selected  --
	if (Pls().HasSelected())
	for (int i=0; i < len; ++i)
	{
		const auto& t = Pls().GetTrackVis(i);
		bool btw = t.Btw().sel;
		if (t.IsSelected() || btw)
		{
			GetCur(i, i + yr, 1);
			Rect(xk1, float(c1), xk2,
				btw ? float(c2-c1)*0.4f : float(c2-c1), TX_PlsSel, true);
	}	}

	
	///  all tracks  rating  `^
	if (v.sldr.drawRate && !bFind && !Pls().HasSelected())
		for (int i=0; i < len; ++i)
		{
			const auto& t = Pls().GetTrackVis(i);
			const int rate = t.GetRate(), r = rate + Ratings::cntMin;
			if (rate != 0)
			{
				const auto c = Ratings::clrBck[r];
				GetCur(i, i + yr, 1);
				Rect(xk1, float(c1), xk2, float(c2-c1+1),
					Ratings::GetTex(rate), true,
					c[0]*2/3, c[1]*2/3, c[2]*2/3);  //par
		}	}
	
	//  todo: fill texture in Update() and just draw once
}
