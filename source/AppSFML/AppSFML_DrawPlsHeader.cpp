#include "AppSFML_Draw.h"
#include "../System/Utilities.h"
using namespace std;  using namespace sf;


//  Pls header
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPls_Header()
{
	const int low = Pls().GetFilter(true), high = Pls().GetFilter(false);

	//if (yB_pli >= view.ySize)  return;
	bool sel = Pls().HasSelected();
	
	//  get
	const Stats& stats = 
		sel ? Pls().statsSel :
		bAllStats ?
			(bFullStats ? allFull : all) :
			(bFullStats ? Pls().statsAll : Pls().stats);
	const Stats& ful =
		bAllStats ? allFull : Pls().statsAll;

	uint dirs, files, size, time;
	dirs = stats.GetDirs();  // todo: count dirs on sel?
	files = stats.GetFiles();
	size = stats.GetSize() / 1000000;
	time = stats.GetTime();


	//  filter  ` Y *
	int x = xM_pl_filt, y = yB_pl_inf;
	const EFont fnt = Fnt_Player;

	Clr(130,160,195);
	str = Ratings::GetVis(low);
	Text(fnt, x -11-2, y+1);
	str = Ratings::GetVis(high);
	Text(fnt, x +13+2, y+1);

	Clr(50,80,115);  //  Y icon
	str = String(Uint32(0x25BE));
	Text(fnt, x + 2, y-4);  str = "Y";
	Text(fnt, x + 1, y-1);


	//  clr
	if (sel)  // sel bckgr
	{	Rect(0, yB_pl_inf, set.view.wnd.xSize, yB_pl-yB_pl_inf, TX_PlsSel, true, 60,60,60);
		Clr(160,210,210);
	}else
	if (bAllStats){  if (bFullStats)  Clr(160,170,190);  else  Clr(150,150,190);  }
	else		  {  if (bFullStats)  Clr(120,150,190);  else  Clr(100,130,160);  }


	//  filter  % files
	if (stats.GetFiles() < ful.GetFiles())
	{
		float p = 100.f * stats.GetFiles() / ful.GetFiles();
		str = f2s(p, 0,2) + "%";
		Text(fnt, x + 32, y);
	}


	//  Info  Total dirs, files, size, time
	//----------------------------------------------------------------

	//  |Dirs Files Size
	y = yB_pl_inf;
	string ss = size2s(size);
	if (dirs == 0)
	{	str = i2s(files) +"  "+ ss;  Text(fnt, 20, y);  }
	else
	{	str = i2s(dirs) +"  "+ i2s(files) +"  "+ ss;  Text(fnt, 0, y);  }

	//if (Pls().bThrTi /*&& plst->itu < plst->listLen*/)  {  % progress ..
	//	Format("%6.1f%%", 100.f * Pls().itu / float(Pls().listLen));  Text(fnt, xwr, y);  }
	

	//  Total Time|
	str = time2s(time);
	x = xE_pl_inf;  // align right
	int w = Text(fnt, 0, 0, false);
	Text(fnt, x - w, y);
}
