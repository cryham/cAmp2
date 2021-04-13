#include "AudioBass.h"
#include "../Playlist/Track.h"
#include "../Playlist/Playlist.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
#include "../System/defines.h"
using namespace std;


void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	AudioBass* ab = (AudioBass*)user;
	if (!ab->bRepTrk)
	{
		ab->Stop();
		ab->GetPls()->Next();
	}
}

///  Play
/// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
bool AudioBass::Play(Track& trk)
{
	//  rem old
	Stop();
	
	if (!trk.hasTime)
		GetTrackTime(trk);  // for mod and size

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
				Log("Can't play file: " + trk.path.string() + "\n  error code: " + i2s(er)); // + GetErrStr(er);
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
/// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


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
