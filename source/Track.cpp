#include "Track.h"
#include "Util.h"
using namespace std;


Track::Track(fs::path file)
{
    path = file;
    SetNameFromPath();
}

void Track::SetNameFromPath()
{
    //name = path.generic_wstring();
    name = path.filename().filename().string();
	//  ext
	ext = path.extension();
	ext = ext.substr(1);  // no .
	strupper(ext);

	//mod = AudioBass::IsModFile  ..?

	//  rem ext from name
	size_t i = name.find_last_of(".");
	if (i != string::npos)
		name = name.substr(0, i);
	
	// todo: get rating, bookmark from name
}
