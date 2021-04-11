#include "Stats.h"
#include "../Playlist/Track.h"


Stats::Stats()
{
	Clear();
}

void Stats::Clear()
{
	size = 0;
	time = 0.0;
	dirs = 0;
	files = 0;
}

void Stats::AddDir()
{
	++dirs;
}

void Stats::Add(const Track* t)
{
	if (t->IsDir())
		++dirs;
	else
	{
		++files;
		size += t->GetSize();
		time += t->GetTime();
	}
}

void Stats::operator += (const Stats& add)
{
	size += add.size;
	time += add.time;
	dirs += add.dirs;
	files += add.files;
}
