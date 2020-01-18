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
	sscanf(s,"%d|%d|%d|%d|%d", &filterLow, &filterUp, &cur, &ofs, &play);
	
	fs::path prevPath, a;
	while (!fi.eof())
	{
		fi.getline(s,MP,'|');  /// path only, utf8
		if (strlen(s) > 0)
		{
			if (s[0] == '<')  // short, add prev path
			{	string f(s);
				f = f.substr(1);
				a = prevPath / f;
			}else
				a = s;
				
			Track t(a);
			prevPath = t.path.parent_path();
			
			// time,size
			fi.getline(s,MP,'|');  t.time = s2d(s);  t.gotTime = true;
			fi.getline(s,MP,'|');  t.size = s2i(s);

			// hide,rate,bokm,mod
			fi.getline(s,80);	int h=0,r=0, b=0, m=0;
			sscanf(s,"%d|%d|%d|%d", &h, &r, &b, &m);
			t.hide=h;  t.rate=r;  t.bokm=b;  t.mod=m;
			
			tracks.push_back(move(t));
		}
	}
	fi.close();
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
	of << filterLow <<'|'<< filterUp <<'|'<< cur <<'|'<< ofs <<'|'<< play <<"\n";
	
	for (int i=0; i < Length(); ++i)
	{
		const Track& t = tracks[i];
		if (i > 0 && t.path.parent_path() == tracks[i-1].path.parent_path())
			//  same path, only file
			of << '<' << t.path.filename().u8string();
		else
			of << t.path.u8string();

		of << '|'<< t.time <<'|'<< t.size;
		of << '|'<< (int)t.hide;
		of << '|'<< (int)t.rate <<'|'<< (int)t.bokm;
		of << '|'<< t.mod ? '1':'0';
		of << "\n";
	}
	of.close();
	return true;
}

//------------------------------------------------
void Playlist::Clear()  // defaults
{
	tracks.clear();
	cur = 0;  ofs = 0;  play = 0;
	bDraw = true;
	filterLow = -2;  filterUp = 5;
	
	allSize = 0;
	allTime = 0.0;  allDirs = 0;  allFiles = 0;
}