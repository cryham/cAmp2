#pragma once
#include <string>
#include <filesystem>
namespace fs = std::filesystem;


class Track
{
	friend class AudioBass;
	friend class Playlist;
protected:

    //  shown in gui,  got from path
    std::string name;  // no rating, bookm
	std::string ext;   // uppercase, no dot .

    //  file on disk, saved in playlist
    fs::path path;
	//std::string path2;  // subdir-1
	
	//  time
	bool gotTime = false;  // needs update
	double time = 0.0;  // in seconds
	uintmax_t size = 0;

	//  flags
	bool dir = false;
	bool disabled = false;  // can't play

	///  extra  ----
	bool mod = false;  // xm, mod files etc.
	char hide = 0, sel = 0;  // todo:

public:
	char rate = 0;  // rating
	char bookm = 0;  // bookmark
	//byte srch;   // match
	
	//bool vis = true;  // visible, after filtering etc

public:
    Track(fs::path file, bool dir1=false);

	//  get for draw
    const std::string& GetName() const
    {   return name;  }

	bool GotTime() const
	{	return gotTime;  }
	
	double GetTime() const
	{	return time;  }

	double GetSize() const
	{	return size;  }
	
	bool IsDisabled() const
	{	return disabled;  }

	bool IsDir() const
	{	return dir;  }
	
	void SetNameFromPath();
	void CleanNameRating();
	static void GetNameRating(const std::string& name, char& pRate, char& pBookm);
};


///  rating chars in name
const int chFnAll = 7;  // special -2 -- 
const char cFnCharRates[chFnAll] = {'=','-', '`','^','~','+','#'};
const char cFnNumRates[chFnAll] = {-3,-1, 1,2,3,4,5};

const int cRmin = 3/*-*/, cRmax = 5/*+*/, chRall = cRmin+cRmax+1;
const static std::string chFRateAdd[chRall] = {"=","--","-","","`","^","~","+","#"};  // rename add, file name
const static std::string chFRateVis[chRall] = {"=","--","-","","`","^","~","+","*"};  // draw text, sybols
const static int cRateMin = -cRmin, cRateMax = cRmax;
