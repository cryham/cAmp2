#include "Playlist.h"
#include "../System/defines.h"
#include "../System/Utilities.h"
using namespace std;


//  list move (keys)
//--------------------------------------------------------------------
void Playlist::Cur()
{
	int all = LengthVis()-1;
	iCur = mia(0, all, iCur);
	bDraw = true; 
}
void Playlist::Ofs()
{
	int all = LengthVis();
	if (iOfs > all-iLinVis)  iOfs = all-iLinVis;  //  no view past last track
	if (iOfs < 0)  iOfs = 0;  //  cur stays in view
	bDraw = true;
}
void Playlist::Up		(int m){  iCur -= m;  Cur();  int d= iCur-iOfs;            if (d < 0) {  iOfs += d;  Ofs();  }  }
void Playlist::Dn		(int m){  iCur += m;  Cur();  int d= iCur-iOfs-iLinVis+1;  if (d > 0) {  iOfs += d;  Ofs();  }  }

void Playlist::PgOfsUp	(int m){  iOfs -= m;  Ofs();  int d= iCur-iOfs-iLinVis+1;  if (d > 0) {  iCur -= d;  Cur();  }  }
void Playlist::PgOfsDn	(int m){  iOfs += m;  Ofs();  int d= iCur-iOfs;            if (d < 0) {  iCur -= d;  Cur();  }  }

void Playlist::PgUp		(int m){  iCur -= m;  iOfs -= m;  Cur();  Ofs();  }
void Playlist::PgDn		(int m){  iCur += m;  iOfs += m;  Cur();  Ofs();  }

void Playlist::Home (int m)
{
	switch(m)
	{
	case 2:  iCur = 0;  iOfs = 0;  break;  // list top
	case 1:  iCur = iOfs;  break;  // view

	case 0:  // dir
		do  --iCur;
		while (iCur-1 > 0 && !visible[iCur].dir);
		Up(0);  break;
	}
	bDraw = true;
}

void Playlist::End (int m)
{
	int all = LengthVis();
	switch(m)
	{
	case 2:  iCur = all-1;  iOfs = all-1;  Cur();  Ofs();  break;  // list end
	case 1:  iCur = iOfs+iLinVis-1;  Cur();  break;  // view

	case 0:  // dir
		do  ++iCur;
		while (iCur+1 < all && !visible[iCur].dir);
		Dn(0);  break;
	}
	bDraw = true;
}


//  Select
//--------------------------------------------------------------------
void Playlist::Pick(int cur)
{
	iCur = cur;  Cur();  ylastSel = cur;
}

void Playlist::sel(int ivL, bool sel)
{
	Track& t = GetTrackVis(ivL);
	if (t.IsDir())  return;

	if (sel){	t.sel = true;   stSel.Add(&t);  }  // select
	else	{	t.sel = false;  stSel.Sub(&t);  }  // unselect
}

void Playlist::SelDir(int cur)  // select dir
{
	/*iCur = cur;  Cur();  int lc = iCur;
	if (!GetTrackVis(lCur).isDir())  return;
	lCur++;
	while (lCur < listLen && !GetTrackVis(Cur).isDir())
	{
		sel(lCur, !GetTrackVis(lCur).sel);
		lCur++;
	}
	lCur = lc;*/
}

void Playlist::SelRange(int cur, bool unselect)
{
	if (ylastSel <= 0)  return;

	int a = min(ylastSel,cur), b = max(ylastSel,cur);

	if (unselect)
	{	for (int i = a; i <= b; ++i)
			if (GetTrackVis(i).sel)  sel(i, 0);
	}else{
		for (int i = a; i <= b; ++i)
			if (!GetTrackVis(i).sel)  sel(i, 1);
	}
	Pick(cur);
}

void Playlist::Select1(int cur)
{
	if (GetTrackVis(cur).IsDir())
		SelDir(cur);
	else
	{	sel(cur, 1 - GetTrackVis(cur).sel > 0);
		Pick(cur);
	}
}

//  unselect all
void Playlist::UnSel()
{
	for (int i=0; i < LengthAll(); ++i)
		tracks[i].sel = false;
	unSel0();
}

void Playlist::unSel0()
{
	stSel.Clear();
}
