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

	case 0:
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

	case 0:
		do  ++iCur;
		while (iCur+1 < all && !visible[iCur].dir);
		Dn(0);  break;
	}
	bDraw = true;
}
