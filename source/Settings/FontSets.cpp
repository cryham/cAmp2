#include "FontSets.h"
using namespace std;


SFontSet::SFontSet()
{
	Default();
}
void SFontSet::Default()
{
	name = "default";
	for (int i=0; i < Fnt_ALL; ++i)
	{
		auto&f = fnt[i];
		f.name =
			i == Fnt_TimeBig || i == Fnt_Times ?
			"NotoMono.ttf" :
			"DejaVuLGCSans.ttf";
		switch (i)
		{
		case Fnt_Player:	f.size = 14;  break;
		case Fnt_TimeBig:	f.size = 20;  break;
		case Fnt_Tabs:		f.size = 14;  break;
		case Fnt_Tracks:	f.size = 14;  break;
		case Fnt_Times:		f.size = 15;  break;
		default:			f.size = 15;  break;
		}
		f.bold = false;
		f.height = f.size + 3;
	}
}
