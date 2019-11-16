#include "Track.h"


Track::Track(fs::path file)
{
    path = file;
    SetNameFromPath();
}

void Track::SetNameFromPath()
{
    //name = path.generic_wstring();
    name = path.filename().filename().string();
}
