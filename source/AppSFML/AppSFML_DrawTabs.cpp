#include "AppSFML_Draw.h"
using namespace std;  using namespace sf;


//  playlist Tabs
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawTabs()
{
	const ViewSet& v = set.view;
	
	Clr(50,80,115);  // ^ v btns
	str = String(Uint32(0x25B4));
	Text(Fnt_Tabs, v.wnd.xSize - 14, yB_tabs +1);
	str = String(Uint32(0x25BE));
	Text(Fnt_Tabs, v.wnd.xSize - 14, yE_tabs-yH_tabs -1);

	/* multi line  up,dn */
	int ytB = yB_tabs+1;
	if (ytB >= v.wnd.ySize)  return;  // fullscr vis
	
	int ntab = vPls.size();
	if (ntab == 0)  return;

	int x,y, a = v.tabs.ofs;
	for (y=0; y < v.tabs.yRows; ++y)
	for (x=0; x < v.tabs.xCols; ++x,++a)
	if (a < ntab)
	{
		const auto& pls = vPls[a];
		float yt = yB_tabs + y*yH_tabs;
		float x1 = x*xW_tabs, x2 = xW_tabs;
		
		Rect(x1, yt, x2, yH_tabs, TX_Black);  // clear backgr
		
		//  bookm
		int b = pls.bookm, d;
		if (b > 0)
		{	int tex = TX_TabB1 + b-1;
			d = set.dimTabBck;
			Rect(x1, yt, x2, yH_tabs, ETexUV(tex), false,
				 pls.bck[0]*d/16, pls.bck[1]*d/16, pls.bck[2]*d/16);
		}
		
		//  cur, add
		d = set.dimTabTxt;
		Clr(pls.txt[0]*d/16, pls.txt[1]*d/16, pls.txt[2]*d/16);
		if (a == plsId)   {  Clr(220,240,255);  Rect(x1, yt, x2, yH_tabs, TX_TabCur, true);  }
		if (a == plsPlId) {  Clr(240,240,255);  Rect(x1, yt, x2, yH_tabs, TX_TabPlay, true); }  // playing
		if (a == plsSelId){  Clr(170,240,170);  Rect(x1, yt, x2, yH_tabs, TX_TabSel, true);  }  // selected
		if (a == queId)   {  Clr(210,210,210);  Rect(x1, yt, x2, yH_tabs, TX_TabQue, true);  }  // queue
		//if (a==nTabMov)  Rect(x1, yt, x2, yH_tabs, TX_TabCur, true);  // moving
		if (bFind && pls.GetFound() > 0)
						{	Clr(70,240,70);  Rect(x1, yt, x2, yH_tabs, TX_TabCur, true);  }  // find

		//  text
		str = pls.name;
		const int w = Text(Fnt_Tabs, 0,0, false);
		int xc = max(0, (xW_tabs - w)/2);  //center
		Text(Fnt_Tabs, x*xW_tabs + xc, ytB + y*yH_tabs);
	}
}
