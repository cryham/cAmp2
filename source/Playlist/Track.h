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
	bool sel = false;

	int rateInName = 0;   // to check if user changed
	int bookmInName = 0;  // since last rename rating

	int rate = 0;   // rating
	int bookm = 0;  // bookmark
	bool found = false;     // find match

	//  set in UpdateVis
	bool visible = true;  // visible, after filtering etc.
	int idPlayVis = 0;  // id to tracksVis
	int idDir = 0;  // id to Playlist dirs[]

	void SetNameFromPath();
	void CleanNameRating();
	
	bool NeedsRename() const  // user changed
	{	return
		rate  != rateInName ||
		bookm != bookmInName;
	}
public:
	Track(fs::path file, bool dir1 = false);

	//  getters for draw  ----
	const std::string& GetName()    const {   return name;  }
	const std::string  GetPath()    const {   return path.u8string();  }
	const std::string& GetParent()  const {   return parent;  }
	const std::string& GetParent2() const {   return parent2;  }

	bool HasTime()    const	{   return hasTime;  }
	double GetTime()  const	{   return time;  }
	double GetSize()  const	{   return size;  }

	char GetRate()    const	{   return rate;  }
	char GetBookmark()const {   return bookm;  }
	EHide GetHide()   const	{   return hide;  }

	bool IsDir()      const	{   return dir;  }
	bool IsDisabled() const	{   return disabled;  }
	bool IsFound()    const	{   return found;  }
	bool IsSelected() const	{   return sel;  }
};
