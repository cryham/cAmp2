#pragma once
#include <vector>
#include "Audio.h"
#include "../../bass/src/bass.h"


class AudioBass : public Audio
{
    HSTREAM chPl = 0;   // e.g. ogg mp3
    HMUSIC chMod = 0;   // e.g. xm mod
    DWORD ch() {  return chMod ? chMod : chPl;  }
    HSYNC chSync = 0;
	
	std::vector<float> fft;

public:
    AudioBass();

    void Init() override;
    void Destroy() override;

    bool Play() override;
    void Stop() override;
};
