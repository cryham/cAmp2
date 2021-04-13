#include "AudioBass.h"
#include "../Playlist/Track.h"
#include "../Playlist/Playlist.h"
#include "../System/FileSystem.h"
#include "../System/defines.h"
#include <iostream>
#include <cmath>
using namespace std;


const uint AudioBass::ciFFTSize[ViewSet::FFTSizes] = {
	BASS_DATA_FFT512, BASS_DATA_FFT1024, BASS_DATA_FFT2048, BASS_DATA_FFT4096, BASS_DATA_FFT8192};


///  Get audio data for Visualization
///. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void AudioBass::GetVisualData(int size, const ViewSet& view)
{
	//DWORD chan = bPlay ? ch() : chRec;
	DWORD chan = ch();

	const auto& v = view.vis;
	int xw1 = view.wnd.xSize+1;
	
	if (v.eType == VisT_FFT || v.eType == VisT_Spect)
	{
		BASS_ChannelGetData(chan, (void*)fft, ciFFTSize[v.iFFT_Size] );

		for (int x=0; x < xw1; ++x)
		{
			float f = fft[x+1];  if (f<0.000001f) f=0.000001f;
			float y = -log10(f) * v.fFFT_Mul /255.f -0.1f;  //par

			y = mia(0.f,1.f, y);  vis[x] = y;
		}
	}
	else if (v.eType == VisT_Osc)
	{
		BASS_ChannelGetData(chan, wav, 2*2*xw1*sizeof(short));
		uint a = 0;
		for (int x=0; x < xw1; ++x)
		{
			int i = (wav[a++] + wav[a++]) / 2;  a+=2;  // stereo to mono
			float y = 0.5f + i/65536.f;
			vis[x] = y;
			//vis[x] = (0.5f - 0.5f*sin(float(x)/xw1*2*3.14f) );  // test
		}
	}
	
	/*  Line meter =
	{
		float fL = LOWORD(BASS_ChannelGetLevel(chan)) / 128.f;
		float fR = HIWORD(BASS_ChannelGetLevel(chan)) / 128.f;
	}/**/
}


//  Track time
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void AudioBass::GetTrackTime(Track& t)
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
	t.size = FileSystem::Size(t.path.string());
	t.hasTime = true;
}


//  seek, change playing position
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
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
