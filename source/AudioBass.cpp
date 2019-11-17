#include <iostream>
#include "AudioBass.h"
using namespace std;


AudioBass::AudioBass()
{

}

void AudioBass::Init()
{
    cout << "Init Sound: bass ----" << endl;

    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
        cerr << "Incorrect bass ver ----" << endl;

    //  find all devs
    //sDevs.clear();
    BASS_DEVICEINFO di;
    int devs = -1;
    for (int n=0; BASS_GetDeviceInfo(n,&di); n++)
    if (di.flags & BASS_DEVICE_ENABLED)
    {
        //sDevs.push_back(di.name);
        devs = n;
        cout << "Device " << n << ": " << di.name <<
                (di.flags & BASS_DEVICE_DEFAULT ? " (default)": "") << endl;
    }
    int nDev = 1;  /// par
    //int nDev = devs-1;

    //  Init
    DWORD fl = BASS_DEVICE_FREQ;
    int nFreq = 44100;  /// par
    if (!BASS_Init(nDev,nFreq, fl, NULL,NULL))
        cout << "Can't initialize bass" << endl;

    //  get freq info-
    cout << "Info ----\n";
    BASS_INFO in;
    BOOL b = BASS_GetInfo(&in);  maxFreq = 44100;
    if (b)
    {	cout << "speakers: " << in.speakers << endl;
        cout << "max freq: " << in.maxrate << endl;;  maxFreq = in.maxrate;
        cout << "cur freq: " << in.freq << endl;;
    }

}

void AudioBass::Destroy()
{
    BASS_Free();
    cout << "Destroyed Sound: bass ----" << endl;
}


void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    //cAmp* pAmp = (cAmp*)user;
    //if (!pAmp->bRep1)  pAmp->Next();
}


bool AudioBass::Play()
{
    //. rem old
    Stop();

    //  get name
    string fname = "../../../zm/c.mp3";

    //if (tkPl->mod)
    /*if (cExt::Find(tkPl->ext) < 0)
    {   // load mod
        chMod = BASS_MusicLoad(FALSE, fname.c_str(), 0,0,
            BASS_MUSIC_STOPBACK | BASS_MUSIC_AUTOFREE | //?
            BASS_MUSIC_RAMPS | BASS_MUSIC_SINCINTER |  // quality todo opt
            BASS_MUSIC_FT2PAN | BASS_MUSIC_FT2MOD | //BASS_MUSIC_PT1MOD | //MODBASS_MUSIC_SURROUND |
            (bRep1? BASS_SAMPLE_LOOP: 0) | BASS_MUSIC_PRESCAN, 1);
    }else*/
    {   // create stream
        chPl = BASS_StreamCreateFile(FALSE, fname.c_str(), 0,0,
            (/*bRep1*/false? BASS_SAMPLE_LOOP: 0) | BASS_STREAM_AUTOFREE);
    }

    if (!chPl && !chMod)
    {
        int er = BASS_ErrorGetCode();
        cout << "er " << er << endl;
        switch (er)
        {
            case BASS_ERROR_FILEOPEN:
            case BASS_ERROR_FILEFORM:  // unsup format
            {
                //  cant open, not found
                //tkPl->dis = 1;
                //if (bNextPrev)	Next();  else  Prev();
                cout << "disabled" << endl;
            }	return false;

            default:  // other
            {
                int er = BASS_ErrorGetCode();
                cout << "Can't play file: " + fname + "\n  error code: " << er + ": " << endl; // + GetErrStr(er);
            }	return false;
        }
    }
    //else  tkPl->dis = 0;

    //  sync reaching end - for play next
    chSync = BASS_ChannelSetSync(ch(), BASS_SYNC_END/*or BASS_SYNC_FREE*/, 0, EndSync, 0/*this*/);

    //  get file info
    int bitrate = 0, size = 3200100;
    QWORD bytes = BASS_ChannelGetLength(ch(), BASS_POS_BYTE);
    if (bytes > 0)
    {
        timeTrack = BASS_ChannelBytes2Seconds(ch(), bytes);
        bitrate = 0.008 * double(size) / timeTrack;
    }
    else
        timeTrack = 0.0;

    //  ext  kbps  freq  size
    BASS_CHANNELINFO info;	BASS_ChannelGetInfo(ch(), &info);
    cout << //"%4d %2d  %3.1f"
        bitrate/*info.origres*/ << " " <<
        info.freq/1000 << " " <<
        double(size-44)/1000000.0 << endl;

    //  play
    float fVol = 0.1f;  //par
    BASS_ChannelSetAttribute(ch(), BASS_ATTRIB_VOL, fVol);  //,bal
    BASS_ChannelPlay(ch(), TRUE);
    bPlaying = true;  bPaused = false;
    return true;
}


void AudioBass::Stop()
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
}
