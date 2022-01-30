#include "App.h"
#include "../System/Utilities.h"


//								. . . . .  Tabs  . . . . .
void App::TabNext(int i, bool row, bool ofs)
{
	Pls().Save();
	const ViewSet& v = set.view;
	const int l = vPls.size()-1;
	if (ofs)
	{	int d = row ? i : v.tabs.xCols*i;
		return;
	}
	//Pls().Save();
	plsId += (row && l > v.tabs.xCols) ? v.tabs.xCols*i : i;
	if (i > 0)
	{	if (plsId > l)  plsId -= l+1;  }
	else
		if (plsId < 0)	plsId += l+1;
	//plsChg();
	Redraw();
}

//  util
/*void cAmp::plsChg(int clrSel)
{
	pls = vPlst[plsId];  updSelId(clrSel);
}
void cAmp::plsPlChg(int id)
{
	plsPlId = id;  plsPl = vPlst[plsPlId];
}*/

void App::updSelId(int clear)
{
	if (Pls().HasSelected())
		plsSelId = plsId;
	else
	if (clear)
		plsSelId = -1;
}

void App::clrSelId()
{
	if (plsId == plsSelId)
	  plsSelId = -1;
}


///  Move tab  . . . . . . . 
void App::TabMove(int n)
{
	Playlist p = Pls();  // copy
	if (vPls.size() <= 1 || n == plsId)  return;
	/**/int e=0;  if (plsPlId == plsId)  e=1;  // move pl
	/**/int s=0;  if (plsSelId == plsId) s=1;  // move sel
	
	int l = vPls.size()-1; //last
	if (plsId == l)
		vPls.pop_back();
	else
		vPls.erase(vPls.begin() + plsId);

	if (n == l)  {
		vPls.emplace_back(p);  plsId = vPls.size()-1;  }
	else  {
		vPls.insert(vPls.begin() + n, p);  plsId = n;  }
	//plsChg();

	/**if (e==1)  plsPlChg(plsId);  else
	for (int i=0; i<=l; ++i)  // find plsPl
		if (plsPl==vPlst[i])  plsPlChg(i);
	/**if (s==1)  updSelId(1);  else  //{  plsSelId = plsId;  plsSel = vPlst[plsPlId];  }  else
	for (int i=0; i<=l; ++i)  // find plsPl
		if (plsSel==vPlst[i])  {  plsSelId = i;  plsSel = vPlst[plsSelId];  }/**/
}

///  New tab  . . . . . . . 
void App::TabNew(int m)
{
	//Pls().Save();
	Playlist pl(i2s(++set.cntrPls));

	if (m==2)
	{	//  ctrl- at end
		vPls.emplace_back(pl);
		plsId = vPls.size()-1;  //pls = vPlst[plsId];  // sel
	}else
	{	//  none- after cur   alt- before cur   shift- at begin
		vPls.insert(vPls.begin()+
					(m==-2? 0: (plsId+ (m==-1?0:1))), pl);
		// pl +-
		if (m==-2 || m==1 && plsId < plsPlId
				 || m==-1 && plsId <= plsPlId)
		{  plsPlId++;  /*plsPl = vPlst[plsPlId];*/  }  // pl
		
		// sel +-
		if (plsSelId != -1)
		if (m==-2 || m==1 && plsId < plsSelId
				 || m==-1 && plsId <= plsSelId)
		{  plsSelId++;  /*plsSel = vPlst[plsSelId];*/  }  // sel
		
		// cur +-
		if (m==-2)  plsId=0;  else
		if (m==1)  plsId++;
		//plsChg();
	}
}
	
///  Close tab  . . . . . .
void App::TabCloses()
{
	if (vPls.size() <= 1)  return;  //else plst.Clear();
	//Pls().Save();
	//**/int e=0;  if (plsPl == pls)  e=1;  // close pl
	//**/int s=0;  if (plsSel == pls) s=1;  // close sel
	if (plsId <= plsPlId)  plsPlId--;  // pl-  after del
	if (plsSelId > 0 &&
		plsId < plsSelId)  {  plsSelId--;  // sel-
			/*plsSel = vPlst[plsSelId];*/  }
	
	if (plsId == vPls.size()-1)
	{	vPls.pop_back();  plsId--;  }
	else
	{	vPls.erase(vPls.begin() + plsId);  }
		
	//pls = vPlst[plsId];  //plsChg(1);
	//**/if (e==1)  plsPlId = plsId;
	//**/if (s==1)  updSelId(1);
	//plsPl = vPlst[plsPlId];
}		
