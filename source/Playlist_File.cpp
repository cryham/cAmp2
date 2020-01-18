#include "Playlist.h"
#include "FileSystem.h"
#include "Util.h"
#include <string.h>
#include <stdio.h>
using namespace std;
											 ///\\\  List  \\\///

///------------------------------------------------  Load  ------------------------------------------------
bool Playlist::Load()
{
	#define MP 320  // max path len
	char s[MP+2],ss[MP+2],n[MP+2],e[22];
	
	string p = FileSystem::Playlists() + name + ".cp";
	ifstream fi;
	fi.open(p.c_str(), ios_base::in|ios_base::binary);
	if (fi.fail())
	{	Error(string("!Load playlist: Can't open file: ")+p);  return false;  }

	fi.getline(s,20);  // type
	if (strncmp(s,"cAmpPls",7)!=0)
	{	Error("!Load playlist: Not cAmpPls");  return false;  }
	
	fi.getline(s,60);  // header vars
	sscanf(s,"%d|%d|%d|%d|%d", &filterLow, &filterUp, &cur, &ofs, &play);
	
	tracks.clear();
	while (!fi.eof())
	{
		fi.getline(s,MP,'|');  /// path only, utf8
		if (strlen(s) > 0)
		{
			//if (s[0] != '<')  // add prev path todo:..
			Track t(s);
			
			// time,size
			fi.getline(s,MP,'|');  t.time = s2d(s);
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

///------------------------------------------------  Save  ------------------------------------------------
bool Playlist::Save()
{
	if (name.empty())
		return true;

	ofstream of;
	string p = FileSystem::Playlists() + name + ".cp";
	of.open(p.c_str(), ios_base::out|ios_base::binary|ios_base::trunc);
	if (of.fail())
	{	Log("Can't save playlist, file: "+p);  return false;  }
	
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

		of << '|'<< t.time <<'|'<< t.size <<'|';
		of << '|'<< (int)t.hide;
		of << '|'<< (int)t.rate <<'|'<< (int)t.bokm;
		of << '|'<< t.mod ? '1':'0';
		of << "\n";
	}
	of.close();
	return true;
}

//------------------------------------------------  clear
void Playlist::Clear()  // defaults
{
	//destroyThr();
	tracks.clear();  //listLen=0;  //? numSel = 0;
	cur = 0;  ofs = 0;  play = 0;
	bDraw = true;
	filterLow = -2;  filterUp = 5;
	
	allSize = 0;
	allTime = 0.0;  allDirs = 0;  allFiles = 0;
}
