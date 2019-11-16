#include <iostream>

#include "FileSystem.h"
#include "Playlist.h"
using namespace std;


int main()
{
    unique_ptr<Playlist> p = make_unique<Playlist>();
    p->AddDir("../../../zm");

    const auto& tracks = p->GetTracks();
    for (const auto& trk: tracks)
        cout << trk.GetName() << endl;

    return 0;
}
