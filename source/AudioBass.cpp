#include "AudioBass.h"
#include "FileSystem.h"
#include "Util.h"
#include "Track.h"
#include "Playlist.h"

#include <iostream>
#include <cmath>
#include "def.h"
using namespace std;


///  Track
void AudioBass::GetTrkTime(Track& t)
{
	if (t.hasTime)  return;
	bool mod = IsModFile(t.ext);
	t.mod = mod;

	//  get time length
	DWORD chan;
	if (mod)
		chan = BASS_MusicLoad(FALSE, t.path.c_str(), 0,0,
			BASS_MUSIC_NOSAMPLE | BASS_MUSIC_PRESCAN, 1);
	else
		chan = BASS_StreamCreateFile(FALSE, t.path.c_str(), 0,0,0);

	if (chan)
	{
		QWORD bytes = BASS_ChannelGetLength(chan, BASS_POS_BYTE);
		double time = BASS_ChannelBytes2Seconds(chan,bytes);
		t.time = time;
		t.disabled = false;
	}else
	{
		t.time = 0.0;
		t.disabled = true;
	}

	if (mod)
		BASS_MusicFree(chan);
	else
		BASS_StreamFree(chan);
	
	//  size
	t.size = FileSystem::Size(t.path);
	t.hasTime = true;
}


void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    AudioBass* ab = (AudioBass*)user;
	if (!ab->bRepTrk)
	{
		ab->Stop();
		ab->GetPls()->Next();
	}
}

///  Play  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
bool AudioBass::Play(Track& trk)
{
    //  rem old
    Stop();
	
	if (!trk.hasTime)
		GetTrkTime(trk);  // for mod and size

	if (trk.mod)
    {
		// load mod
        chMod = BASS_MusicLoad(FALSE, trk.path.c_str(), 0,0,
            BASS_MUSIC_STOPBACK | BASS_MUSIC_AUTOFREE | //?
            BASS_MUSIC_RAMPS | BASS_MUSIC_SINCINTER |  // quality todo opt
            BASS_MUSIC_FT2PAN | BASS_MUSIC_FT2MOD | //BASS_MUSIC_PT1MOD | //MODBASS_MUSIC_SURROUND |
            (bRepTrk ? BASS_SAMPLE_LOOP : 0) | BASS_MUSIC_PRESCAN, 1);
    }else
    {   // create stream
        chPl = BASS_StreamCreateFile(FALSE, trk.path.c_str(), 0,0,
            (bRepTrk ? BASS_SAMPLE_LOOP : 0) | BASS_STREAM_AUTOFREE);
    }

    if (!chPl && !chMod)
    {
        int er = BASS_ErrorGetCode();
        switch (er)
        {
            case BASS_ERROR_FILEOPEN:
            case BASS_ERROR_FILEFORM:  // unsup format
                //  cant open, not found
                trk.disabled = true;
                //if (bNextPrev)  Next();  else  Prev();  //pls-> ..
                //Log("disabled");
				return false;

            default:  // other
                int er = BASS_ErrorGetCode();
                Log("Can't play file: " + string(trk.path) + "\n  error code: " + i2s(er)); // + GetErrStr(er);
				return false;
        }
    }else
		trk.disabled = false;

    //  sync reaching end - for play next
    chSync = BASS_ChannelSetSync(ch(), BASS_SYNC_END/*or BASS_SYNC_FREE*/, 0, EndSync, this);

    //  get file info
    int bitRate = 0;
    QWORD bytes = BASS_ChannelGetLength(ch(), BASS_POS_BYTE);
    if (bytes > 0)
    {
        timeTrack = BASS_ChannelBytes2Seconds(ch(), bytes);
        bitRate = 0.008 * double(trk.size) / timeTrack;
    }
    else
        timeTrack = 0.0;

    //  ext  kbps  freq  size
    BASS_CHANNELINFO info;  BASS_ChannelGetInfo(ch(), &info);
	sInfo = //"%4d %2d  %3.1f"
		trk.ext + " " +
        i2s(bitRate/*info.origres*/) + " " +
        i2s(info.freq / 1000) + "  " +
        f2s(double(trk.size - 44) / 1000000.0, 1,3);
	//Log("play " + sInfo);

    //  play
    BASS_ChannelSetAttribute(ch(), BASS_ATTRIB_VOL, 0.001f * iVolume);  //,balance
    BASS_ChannelPlay(ch(), TRUE);
    
	bPlaying = true;  bPaused = false;
	rate = trk.rate;  //-
    return true;
}

void AudioBass::Pause()		///  ||
{
	if (!bPlaying)
	{	/*Play();*/  return;  }
	
	if (bPaused)
	{	BASS_ChannelPlay(ch(), FALSE);  bPaused = false;  }
	else
	{	BASS_ChannelPause(ch());  bPaused = true;  }
}

void AudioBass::Stop()		///  []
{
    if (chPl || chMod)
    {
        BASS_ChannelStop(chPl);
        if (chSync)
            BASS_ChannelRemoveSync(ch(), chSync);
        if (chPl)  BASS_StreamFree(chPl);
        if (chMod) BASS_MusicFree(chMod);
        chPl = 0;  chMod = 0;
    }
	bPlaying = false;  bPaused = false;
	if (GetPls() != nullptr)
		GetPls()->bDraw = true;  //darker cur-
}


///  seek, change playing position  - - - - 

void AudioBass::GetPos()
{
	if (!bPlaying)  return;  //timePlay = 0.0;
	timePlay = BASS_ChannelBytes2Seconds(ch(), BASS_ChannelGetPosition(ch(), BASS_POS_BYTE));
}

void AudioBass::SetPos(bool back, bool slow, bool fast)  //  <<  >>
{
	if (!bPlaying || !pls)  return;
	//  todo:  +- % of track
	double add = slow ? 5 : fast ? 30 : 15;  //vSpdSeek[iSpdSeek].s[slow ? 0 : fast ? 2 : 1].add;
	double pos = BASS_ChannelBytes2Seconds(ch(), BASS_ChannelGetPosition(ch(), BASS_POS_BYTE));
	pos += back ? -add : add;

	if (pos < 0.0)
	{	if (!bRepTrk)  pls->Next(-1);
		pos += timeTrack;  // exact
	}
	if (pos > timeTrack)
	{	pos -= timeTrack;
		if (!bRepTrk)  pls->Next();
	}
	BASS_ChannelSetPosition(ch(), BASS_ChannelSeconds2Bytes(ch(), pos), BASS_POS_BYTE);
}

void AudioBass::SetPosAbs(double pos)	//  <<| >>
{
	if (!bPlaying)  return;
	if (pos > 0.999)  pos = 0.999; //-
	BASS_ChannelSetPosition(ch(), BASS_ChannelSeconds2Bytes(ch(), pos*timeTrack), BASS_POS_BYTE);
}

//  volume
void AudioBass::SetVol(bool back, bool slow, bool fast)  //  ^ v
{
	float add = slow ? 10 : fast ? 100 : 20;  //vSpdVol[iSpdVol].v[slow ? 0 : fast ? 2 : 1] * 0.01f;
	iVolume += back ? -add : add;
	iVolume = mia(0,1000, iVolume);  //tmd = tmD;
	if (bPlaying)
		BASS_ChannelSetAttribute(ch(), BASS_ATTRIB_VOL, 0.001f * iVolume);
}


///  FFT / Osc  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//  get data for audio visualization
void AudioBass::GetVisData(int size, const ViewSet& view)
{
	//DWORD chan = bPlay ? ch() : chRec;
	DWORD chan = ch();

	//  get data
	if (view.eVis == viFFT)
	{
		BASS_ChannelGetData(chan, (void*)fft, ciFFTSize[view.iFFTSize] );

		for (int x=0; x < view.xSize+1; x++)
		{
			float f = fft[x+1];  if (f<0.000001f) f=0.000001f;
			float y = -log10(f) * view.fFFTMul /255.f -0.1f;  //par

			y = mia(0.f,1.f, y);  visA[x] = y;
		}
	}/*else
	if (view.eVis == viOsc)
	{
		BASS_ChannelGetData(chan, wav, 2*2*(view.xSize+1)*sizeof(short));
		uint a = 0;  int w = 1;
			if (bPlay)  w=-1;  // osc -y play, rec y
		for (int x=0; x < view.xSize+1; x++)
		{
			int i = w*(wav[a++] + wav[a++]) / 2;  a+=2;
			float y = 0.5f + i/65536.f;
			visA[x] = y;
			//visA[x] = (0.5f - 0.5f*sin(float(x)/view.xSize*6.28) );  // test
		}
	}*/

	/*  Line meter =
	{
		float fL = LOWORD(BASS_ChannelGetLevel(chan)) / 128.f;
		float fR = HIWORD(BASS_ChannelGetLevel(chan)) / 128.f;
	}/**/
}
