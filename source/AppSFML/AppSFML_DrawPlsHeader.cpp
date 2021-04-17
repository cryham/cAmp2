#include "AppSFML_Draw.h"
#include "../System/Utilities.h"
using namespace std;  using namespace sf;


//  Pls header
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPls_Header()
{
	int low = Pls().GetFilter(true), high = Pls().GetFilter(false);
	Clr(130,160,195);

	//  filter  ` Y *
	int x = xM_pl_filt, y = yB_pl_inf;
	EFont fnt = Fnt_Player;

	str = Ratings::GetVis(low);
	Text(fnt, x -11-2, y+1);
	str = Ratings::GetVis(high);
	Text(fnt, x +13+2, y+1);

	Clr(50,80,115);  //  Y icon
	str = String(Uint32(0x25BE));
	Text(fnt, x + 2, y-4);  str = "Y";
	Text(fnt, x + 1, y-1);

	//  info  Total dirs, files, size, time
	//----------------------------------------------------------------
	//if (yB_pli >= view.ySize)  return;
	
	//  get
	uint dirs, files, size, time;
	const Stats& stats = bAllStats ?
		bFullStats ? allFull : all :
		bFullStats ? Pls().stAll : Pls().stats;
	/*if (Pls().numSel > 0)
	{	dirs = 0;  files = Pls().numSel;
		size = Pls().selSize/1000000;
		time = Pls().selTime;  }
	else/**/
	{	dirs = stats.GetDirs();
		files = stats.GetFiles();
		size = stats.GetSize() / 1000000;
		time = stats.GetTime();
	}
	//  clr
	if (bAllStats){  if (bFullStats)  Clr(160,170,190);  else  Clr(150,150,190);  }
	else		  {  if (bFullStats)  Clr(120,150,190);  else  Clr(100,130,160);  }
	
	//  size
	y = yB_pl_inf;
	string ss = size2s(size);
	if (dirs == 0)
	{	str = i2s(files) +"  "+ ss;  Text(fnt, 20, y);  }
	else
	{	str = i2s(dirs) +"  "+ i2s(files) +"  "+ ss;  Text(fnt, 0, y);  }

	//if (Pls().bThrTi /*&& plst->itu < plst->listLen*/)  {  % progress ..
	//	Format("%6.1f%%", 100.f*Pls().itu/float(Pls().listLen));  Text(fnt, xwr, y);  }
	
	//  total time
	str = time2s(time);
	x = xE_pl_inf;
	int w = Text(fnt, x, y, false);
	Text(fnt, x - w, y);
	
	//  num sel
	//if (Pls().numSel > 0)
	//{	clr(0,1,0.9);  cf->Format("%d", Pls().numSel);  Text(view.wnd.xSize/2+20,yBpli);  }  //Sel:
}
