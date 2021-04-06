#include "Playlist.h"
#include "FileSystem.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <string.h>
#include <stdio.h>
using namespace std;


//  Load
//------------------------------------------------
bool Playlist::Load()
{
	#define MP 1022  // max path len
	char s[MP+2];
	
	string p = FileSystem::Playlists() + name + ".cp";
	ifstream fi;
	fi.open(p.c_str(), ios_base::in|ios_base::binary);
	if (fi.fail())
	{	Error(string("Load playlist: Can't open file: ")+p);  return false;  }

	fi.getline(s,20);  // type
	if (strncmp(s,"cAmpPls2",8))
	{	Error("Load playlist: Not cAmpPls");  return false;  }
	// todo: old cAmpPls?
	
	Clear();
	
	fi.getline(s,60);  // header vars
	sscanf(s,"%d|%d|%d|%d|%d|%d|%f|%f|%f",
		&filterLow, &filterHigh,
		&iCur, &iOfs, &iPlay, &bookm,
		&hue, &sat, &val);
	bookm = mia(0,cBookmarkMax, bookm);
	hue = mia(0.f,1.f, hue);  sat = mia(0.f,1.f, sat);  val = mia(0.f,1.f, val);
	UpdateColor();
	
	fs::path prevPath, a;
	while (!fi.eof())
	{
		fi.getline(s,MP,'|');  // path, utf8
		if (strlen(s) > 0)
		{
			if (s[0] == '<')  // short, add prev path
			{	string f(s);  // file
				f = f.substr(1);
				a = prevPath / f;
				stAll.AddDir();
			}else
				a = s;
				
			Track t(a, false);
			prevPath = t.path.parent_path();
			
			//  time,size
			fi.getline(s,MP,'|');  t.time = s2d(s);  t.hasTime = true;
			fi.getline(s,MP,'|');  t.size = s2i(s);

			//  hide, rate,bookm, mod
			fi.getline(s,80);	int h=0,r=0, b=0, m=0;
			sscanf(s,"%d|%d|%d|%d", &h, &r, &b, &m);
			t.hide=h;  t.rate=r;  t.bookm=b;  t.mod=m;
			
			stAll.Add(&t);
			tracksAll.emplace_back(move(t));
		}
	}
	fi.close();
	
	UpdateVis(false);  // no zoom
	return true;
}

//  Save
//------------------------------------------------
bool Playlist::Save()
{
	if (name.empty())
		return true;

	ofstream of;
	string p = FileSystem::Playlists() + name + ".cp";
	of.open(p.c_str(), ios_base::out|ios_base::binary|ios_base::trunc);
	if (of.fail())
	{	Error("Can't save playlist, file: "+p);  return false;  }
	
	of << "cAmpPls2\n";  // header vars
	of << filterLow <<'|'<< filterHigh <<'|'<<
		iCur <<'|'<< iOfs <<'|'<< iPlay <<'|'<< bookm <<'|'<<
		f2s(hue,3,5) <<'|'<< f2s(sat,3,5) <<'|'<< f2s(val,3,5) <<"\n";
	
	for (int i=0; i < LengthAll(); ++i)
	{
		const Track& t = GetTrackAll(i);
		if (i > 0 && t.path.parent_path() == tracksAll[i-1].path.parent_path())
			//  same path, only file
			of << '<' << t.path.filename().u8string();
		else
			of << t.path.u8string();

		of << '|'<< t.time <<'|'<< t.size;
		of << '|'<< (int)t.hide << '|'<< (int)t.rate <<'|'<< (int)t.bookm;
		of << '|'<< t.mod ? '1':'0';
		of << "\n";
	}
	of.close();
	return true;
}

//------------------------------------------------
void Playlist::Clear()  // defaults
{
	tracksAll.clear();
	tracksVis.clear();
	stats.Clear();
	stAll.Clear();

	iCur = 0;  iOfs = 0;  iPlay = 0;
	iLinVis = 10;  bDraw = true;
	filterLow = cRateMin;  filterHigh = cRateMax;
	bookm = 0;
	hue = 0.f;  sat = 0.f;  val = 0.f;
	iFound = 0;
}


//------------------------------------------------
bool Playlist::DeleteCurFile(bool playNext)
{
	const auto& tv = tracksVis[iCur];
	if (tv.dir)  return false;

	if (playNext)  // play next if deleting current
	if (iCur == iPlayVis)
		if (!Next())  audio->Stop();  //return false;
	
	auto& t = tracksAll[tv.iAll];
	if (!fs::remove(t.path))
		return false;
			
	tracksAll.erase(tracksAll.begin() + tv.iAll);
	UpdateVis();
	return true;
}

void Playlist::DeleteCur()
{
	const auto& tv = tracksVis[iCur];
	if (tv.dir)  return;
	
	tracksAll.erase(tracksAll.begin() + tv.iAll);
	UpdateVis();
}
