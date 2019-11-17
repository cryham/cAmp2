#include <iostream>

#include "FileSystem.h"
#include "Playlist.h"
using namespace std;


int main()
{
    FileSystem::Init();
    cout << FileSystem::GetInfo();

    //LogFile::Create(FileSystem::Config());

    //LoadSettings(FileSystem::Settings());

    //LoadTexture(FileSystem::Data());


    unique_ptr<Playlist> p = make_unique<Playlist>();
    p->AddDir("../../../zm");

    const auto& tracks = p->GetTracks();
    for (const auto& trk: tracks)
        cout << trk.GetName() << endl;

    return 0;
}
