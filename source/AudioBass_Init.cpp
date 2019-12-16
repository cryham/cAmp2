#include "AudioBass.h"
#include "FileSystem.h"
#include "Util.h"
#include <algorithm>
#include <iostream>
using namespace std;


AudioBass::AudioBass()
{
//	fft.resize(10240);
//	visA.resize(10240);
}

//  Ext
bool AudioBass::IsModFile(string ext)
{
	if (find(begin(vExtMod), end(vExtMod), ext) != end(vExtMod))
		return true;
	return false;
}

void AudioBass::FillExt()
{
	//  defaults, for bass alone
	vector<string>
		ve{"ogg","mp3","wav","mp2","aiff","mp1"},
		vm{"mod","xm","it","s3m","mtm","mo3","umx"};
	vExt = ve;
	vExtMod = vm;
}

//  Init Plugins
void AudioBass::InitPlugins()
{
	string p = FileSystem::Data() + "/../plugins";
	Log("-------------------------");
	Log(string("Loading plugins from:\n") + p);
	vector<fs::path> files = FileSystem::ListDir(p);
	
	for (auto& f : files)
	if (f.extension() == ".so")
	{
		HPLUGIN plug;
		if (plug = BASS_PluginLoad(f.c_str(),0))
		{
			//  plugin loaded
			const BASS_PLUGININFO *pinfo = BASS_PluginGetInfo(plug);
			Log(string("  ") + f.filename().c_str());
			for (int a=0; a < pinfo->formatc; ++a)
			{
				Log(string("    ") + pinfo->formats[a].name +" (" + pinfo->formats[a].exts + ")");
				//  split Exts
				string se = pinfo->formats[a].exts, s;
				//cout << se << endl;
				
				copy_if(se.begin(), se.end(), back_inserter(s),
					[](char c){  return /*c!='.'&&*/c!='*'&&c!=';';  } );
				//cout << s << endl;
				
				//  add to vExt
				vector<string> vs = split(s, ".");
				for (auto& e:vs)
				if (!e.empty())
				{
					//cout << e << endl;
					//Log(e);
					vExt.push_back(e);
				}
			}
		}else
			Log(string("Plugin load error:") + f.c_str());
	}
}

///  Init  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
void AudioBass::Init()
{
	Log("Init Sound: bass");
	Log("-------------------------");

    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
        Error("Incorrect bass version");

    //  find all devs
    //sDevs.clear();
    BASS_DEVICEINFO di;
    int devs = -1;
	Log("Devices:");
    for (int n=0; BASS_GetDeviceInfo(n,&di); ++n)
    if (di.flags & BASS_DEVICE_ENABLED)
    {
        //sDevs.push_back(di.name);
        devs = n;
        Log(i2s(n,3) + ": " + di.name +
            (di.flags & BASS_DEVICE_DEFAULT ? " (default)": ""));
    }
    int nDev = 1;  /// par
    //int nDev = devs-1;

    //  Init
    DWORD fl = BASS_DEVICE_FREQ;
    int nFreq = 44100;  /// par
    if (!BASS_Init(nDev,nFreq, fl, NULL,NULL))
        Error("Can't initialize bass");

	FillExt();
	InitPlugins();

    //  get freq info-
	Log("-------------------------");
    Log("Info");
    BASS_INFO inf;
    BOOL b = BASS_GetInfo(&inf);  maxFreq = 44100;
    if (b)
    {	Log("  speakers: " + i2s(inf.speakers));
        Log("  max freq: " + i2s(inf.maxrate));  maxFreq = inf.maxrate;
        Log("  cur freq: " + i2s(inf.freq));
    }
	
	//  ext
	string s = "Ext: ";
	for (auto e:vExt)
		s += e+" ";
	Log(s);
	
	s = "ExtMod: ";
	for (auto e:vExtMod)
		s += e+" ";
	Log(s);
	Log("-------------------------");
}

void AudioBass::Destroy()
{
	Stop();
    BASS_Free();
    Log("Destroyed Sound: bass");
}