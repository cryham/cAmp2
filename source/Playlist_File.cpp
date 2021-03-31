#include "Playlist.h"
#include "FileSystem.h"
#include "Util.h"
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
	sscanf(s,"%d|%d|%d|%d|%d|%d", &filterLow, &filterHigh,
								  &cur, &ofs, &play, &bookm);
	
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
			tracksAll.push_back(move(t));
		}
	}
	fi.close();
	
	Update();
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
		  cur <<'|'<< ofs <<'|'<< play <<'|'<< bookm <<"\n";
	
	for (int i=0; i < LengthAll(); ++i)
	{
		const Track& t = tracksAll[i];
		if (i > 0 && t.path.parent_path() == tracksAll[i-1].path.parent_path())
			//  same path, only file
			of << '<' << t.path.filename().u8string();
		else
			of << t.path.u8string();

		of << '|'<< t.time <<'|'<< t.size;
		of << '|'<< (int)t.hide;
		of << '|'<< (int)t.rate <<'|'<< (int)t.bookm;
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

	cur = 0;  ofs = 0;  play = 0;  bookm = 0;
	bDraw = true;
	filterLow = cRateMin;  filterHigh = cRateMax;
	lin = 10;  bookm = 0;
}
