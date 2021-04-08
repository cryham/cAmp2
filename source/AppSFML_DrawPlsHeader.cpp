#include "AppSFML_Draw.h"
#include "Util.h"
using namespace std;  using namespace sf;


//  Pls header
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPlsHeader()
{
	int low = Pls().GetFilter(true), high = Pls().GetFilter(false);
	Clr(130,160,195);

	//  filter  ` *
	int x = xM_pl_filt, y = yB_pl_inf;
	EFont fnt = Fnt_Info;

	str = GetRateStr(low);
	Text(fnt, x -11-2, y+1);
	str = GetRateStr(high);
	Text(fnt, x +13+2, y+1);

	Clr(50,80,115);  //  Y icon
	str = String(Uint32(0x25BE));
	Text(fnt, x + 2, y-4);  str = "Y";
	Text(fnt, x + 1, y-1);

	//  info  Total dirs, files, bookm*, size, time
	//----------------------------------------------------------------
	//bool bList = Pls().LengthAll() > 0;
	//if (yB_pli >= view.ySize)  return;
	
	//  get
	uint di, fi, si, tm;
	const Stats& st = bAllStats ?
		bFullStats ? allFull : all :
		bFullStats ? Pls().stAll : Pls().stats;
	/*if (Pls().numSel > 0)
	{	aD = 0;  aF = Pls().numSel;
		aSi = Pls().selSize/1000000;
		aTm = Pls().selTime;  }
	else/**/
	{	di = st.GetDirs();
		fi = st.GetFiles();
		si = st.GetSize() / 1000000;
		tm = st.GetTime();
	}
	//  clr
	if (bAllStats){  if (bFullStats)  Clr(160,170,190);  else  Clr(150,150,190);  }
	else		  {  if (bFullStats)  Clr(120,150,190);  else  Clr(100,130,160);  }
	
	//  size
	y = yB_pl_inf;
	string s = size2s(si);
	if (di == 0)
	{	str = i2s(fi) +"  "+ s;  Text(fnt, 20, y);  }
	else
	{	str = i2s(di) +"  "+ i2s(fi) +"  "+ s;  Text(fnt, 0, y);  }
//	if (Pls().bThrTi /*&& plst->itu < plst->listLen*/)  {
//		Format("%6.1f%%", 100.f*Pls().itu/float(Pls().listLen));  Text(fnt, xwr, y);  }//-
	
	//  total time
	str = time2s(tm);
	x = xE_pl_inf;
	int w = Text(fnt, x, y, false);
	Text(fnt, x - w, y);
	
	//  num sel
	//if (Pls().numSel > 0)
	//{	clr(0,1,0.9);  cf->Format("%d", Pls().numSel);  Text(view.wnd.xSize/2+20,yBpli);  }  //Sel:
}
