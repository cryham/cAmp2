#include "AudioBass.h"
#include "FileSystem.h"
#include "Util.h"
#include <iostream>
#include <cmath>
using namespace std;


AudioBass::AudioBass()
{
//	fft.resize(10240);
//	visA.resize(10240);
}

void AudioBass::Init()
{
    Log("Init Sound: bass ----");

    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
        Error("Incorrect bass version");

    //  find all devs
    //sDevs.clear();
    BASS_DEVICEINFO di;
    int devs = -1;
    for (int n=0; BASS_GetDeviceInfo(n,&di); n++)
    if (di.flags & BASS_DEVICE_ENABLED)
    {
        //sDevs.push_back(di.name);
        devs = n;
        Log("Device " + i2s(n) + ": " + di.name +
            (di.flags & BASS_DEVICE_DEFAULT ? " (default)": ""));
    }
    int nDev = 1;  /// par
    //int nDev = devs-1;

    //  Init
    DWORD fl = BASS_DEVICE_FREQ;
    int nFreq = 44100;  /// par
    if (!BASS_Init(nDev,nFreq, fl, NULL,NULL))
        Error("Can't initialize bass");

    //  get freq info-
    Log("Info ----");
    BASS_INFO in;
    BOOL b = BASS_GetInfo(&in);  maxFreq = 44100;
    if (b)
    {	Log("speakers: " + in.speakers);
        Log("max freq: " + in.maxrate);  maxFreq = in.maxrate;
        Log("cur freq: " + in.freq);
    }

}

void AudioBass::Destroy()
{
	Stop();
    BASS_Free();
    Log("Destroyed Sound: bass ----");
}


void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    AudioBass* ab = (AudioBass*)user;
	ab->Stop();
    if (!ab->bRep1)
		ab->Next();
}


bool AudioBass::Play()		/// |>
{
    //  rem old
    Stop();

	
    //  get name
//	string fname = "../../../zm/c.mp3";
    string fname = "../../../zm/e.mod";
//	string fname = "../../../zm/s.mp3";

    //if (tkPl->mod)
    if (1)  //fname. cExt::Find(tkPl->ext) < 0)
    {   // load mod
        chMod = BASS_MusicLoad(FALSE, fname.c_str(), 0,0,
            BASS_MUSIC_STOPBACK | BASS_MUSIC_AUTOFREE | //?
            BASS_MUSIC_RAMPS | BASS_MUSIC_SINCINTER |  // quality todo opt
            BASS_MUSIC_FT2PAN | BASS_MUSIC_FT2MOD | //BASS_MUSIC_PT1MOD | //MODBASS_MUSIC_SURROUND |
            (bRep1 ? BASS_SAMPLE_LOOP : 0) | BASS_MUSIC_PRESCAN, 1);
    }else
    {   // create stream
        chPl = BASS_StreamCreateFile(FALSE, fname.c_str(), 0,0,
            (bRep1 ? BASS_SAMPLE_LOOP : 0) | BASS_STREAM_AUTOFREE);
    }

    if (!chPl && !chMod)
    {
        int er = BASS_ErrorGetCode();
        switch (er)
        {
            case BASS_ERROR_FILEOPEN:
            case BASS_ERROR_FILEFORM:  // unsup format
            {
                //  cant open, not found
                //tkPl->dis = 1;
                //if (bNextPrev)  Next();  else  Prev();
                //Log("disabled");
            }	return false;

            default:  // other
            {
                int er = BASS_ErrorGetCode();
                Log("Can't play file: " + fname + "\n  error code: " + i2s(er)); // + GetErrStr(er);
            }	return false;
        }
    }
    //else  tkPl->dis = 0;

    //  sync reaching end - for play next
    chSync = BASS_ChannelSetSync(ch(), BASS_SYNC_END/*or BASS_SYNC_FREE*/, 0, EndSync, this);

    //  get file info
    int bitRate = 0, size = FileSystem::Size(fname);
    QWORD bytes = BASS_ChannelGetLength(ch(), BASS_POS_BYTE);
    if (bytes > 0)
    {
        timeTrack = BASS_ChannelBytes2Seconds(ch(), bytes);
        bitRate = 0.008 * double(size) / timeTrack;
    }
    else
        timeTrack = 0.0;

    //  ext  kbps  freq  size
    BASS_CHANNELINFO info;  BASS_ChannelGetInfo(ch(), &info);
	sInfo = //"%4d %2d  %3.1f"
        i2s(bitRate/*info.origres*/) + " " +
        i2s(info.freq/1000) + "  " +
        f2s(double(size-44)/1000000.0, 1,3);
	//Log("play " + sInfo);

    //  play
    BASS_ChannelSetAttribute(ch(), BASS_ATTRIB_VOL, 0.001f * iVolume);  //,balance
    BASS_ChannelPlay(ch(), TRUE);
    bPlaying = true;  bPaused = false;
    return true;
}

void AudioBass::Pause()		///  ||
{
	if (!bPlaying)
	{	Play();  return;  }
	
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
}


///  change playing position  - - - - 

void AudioBass::getPos()
{
	if (!bPlaying)  return;  //timePlay = 0.0;
	timePlay = BASS_ChannelBytes2Seconds(ch(), BASS_ChannelGetPosition(ch(), BASS_POS_BYTE));
}

void AudioBass::chPos(bool back, bool slow, bool fast)  //  <<  >>
{
	if (!bPlaying)  return;
	//  todo:  +- % of track
	double add = slow ? 5 : fast ? 30 : 15;  //vSpdSeek[iSpdSeek].s[slow ? 0 : fast ? 2 : 1].add;
	double pos = BASS_ChannelBytes2Seconds(ch(), BASS_ChannelGetPosition(ch(), BASS_POS_BYTE));
	pos += back ? -add : add;
	if (pos < 0.0)
	{	if (!bRep1) Prev();  pos += timeTrack;  }  //out exact
	if (pos > timeTrack)
	{	pos -= timeTrack;  if (!bRep1) Next();  }
	BASS_ChannelSetPosition(ch(), BASS_ChannelSeconds2Bytes(ch(), pos), BASS_POS_BYTE);
}

void AudioBass::chPosAbs(double pos)	//  <<| >>
{
	if (!bPlaying)  return;
	if (pos > 0.999)  pos = 0.999; //-
	BASS_ChannelSetPosition(ch(), BASS_ChannelSeconds2Bytes(ch(), pos*timeTrack), BASS_POS_BYTE);
}

//  volume
void AudioBass::chVol(bool back, bool slow, bool fast)  //  ^ v
{
	float add = slow ? 10 : fast ? 100 : 20;  //vSpdVol[iSpdVol].v[slow ? 0 : fast ? 2 : 1] * 0.01f;
	iVolume += back ? -add : add;
	iVolume = mia(0,1000, iVolume);  //tmd = tmD;
	if (bPlaying)
		BASS_ChannelSetAttribute(ch(), BASS_ATTRIB_VOL, 0.001f * iVolume);
}


///  FFT / Osc  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//  get data for audio visualization
void AudioBass::GetVisData(int size)
{
	//DWORD chan = bPlay ? ch() : chRec;
	DWORD chan = ch();

	//  get data
	//if (view.eVis==viFFT)
	{
		BASS_ChannelGetData(chan, (void*)fft, ciFFTSize[/*view.fftSize*/1] );

		for (int x=0; x < /*view.xSize+1*/size+1; x++)
		{
			float f = fft[x+1];  if (f<0.000001f) f=0.000001f;
			float y = -log10(f) * /*view.fftMul*/69.f /255.f -0.1f;  //par

			y = mia(0.f,1.0f, y);	visA[x] = y;
		}
	}/*else
	if (view.eVis==viOsc)
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


void AudioBass::Next()
{
	//pls->
}

void AudioBass::Prev()
{
	
}
