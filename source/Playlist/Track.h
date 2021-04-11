#pragma once
#include <string>
#include <filesystem>
namespace fs = std::filesystem;


enum EHide
{	Hid_None, Hid_Hide, Hid_Show  };

class Track
{
	friend class AudioBass;
	friend class Playlist;
protected:

	//  shown in gui,  got from path with SetNameFromPath
	//e.g.  /parent2/parent/name.ext
	std::string name;  // no rating, bookm
	std::string ext;   // uppercase, no dot .

	//  file on disk, saved in playlist
	fs::path path;
	std::string parent, parent2;  // parent dirs

	//  time
	bool hasTime = false;  // needs update
	double time = 0.0;  // in seconds
	uintmax_t size = 0;

	//  flags
	bool dir = false;
	bool disabled = false;  // can't play

	///  extra  ----
	bool mod = false;  // true for xm, mod files etc.
	EHide hide = Hid_None;
	bool sel = 0;  // todo:

	char rateInName = 0;
	char bookmInName = 0;
public:
	char rate = 0;   // rating
	char bookm = 0;  // bookmark
	bool found = false;     // match

	//  set in UpdateVis
	bool vis = true;  // visible, after filtering etc.
	int idPlayVis = 0;  // id to tracksVis

public:
	Track(fs::path file, bool dir1=false);

	//  get for draw
	const std::string& GetName() const
	{   return name;  }

	const std::string GetPath() const
	{   return path.u8string();  }

	const std::string& GetParent() const
	{   return parent;  }

	const std::string& GetParent2() const
	{   return parent2;  }


	bool HasTime() const
	{	return hasTime;  }

	double GetTime() const
	{	return time;  }

	double GetSize() const
	{	return size;  }


	bool IsDisabled() const
	{	return disabled;  }

	bool IsDir() const
	{	return dir;  }
	
	EHide GetHide() const
	{	return hide;  }


	void SetNameFromPath();
	void CleanNameRating();
	static void GetNameRating(const std::string& name, char& pRate, char& pBookm);
};


///  rating chars in name
const int chFnAll = 7;  // special -2 -- 
const char cFnCharRates[chFnAll] = {'=','-', '`','^','~','+','#'};
const static char* cFnStrRates = "=-`^~+#";
const char cFnNumRates[chFnAll] = {-3,-1, 1,2,3,4,5};

const int cRmin = 3/*-*/, cRmax = 5/*+*/, chRall = cRmin+cRmax+1;
const static std::string chFRateAdd[chRall] = {"=","--","-","","`","^","~","+","#"};  // rename add, file name
const static std::string chFRateVis[chRall] = {"=","--","-","","`","^","~","+","*"};  // draw text, sybols
const static int cRateMin = -cRmin, cRateMax = cRmax;

inline static std::string GetRateStr(int rate)
{	return chFRateVis[rate + cRmin];  }

const int cBookmarkMax = 6;  // %1..%6
