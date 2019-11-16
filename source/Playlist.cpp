#include <vector>
#include "FileSystem.h"
#include "Playlist.h"
using namespace std;


Playlist::Playlist()
{

}

bool Playlist::AddDir(fs::path dir, const EInsert& where)
{

    vector<fs::path> files = FileSystem::ListDir(dir);
    for (const auto& file: files)
    {
        if (fs::is_directory(file))
            continue;

        // check if file is playable..

        Track t(file);
        switch (where)
        {
        case Ins_Cursor:
            //tracks.insert(cur,t);
            break;
        case Ins_Top:
            tracks.push_front(move(t));
        case Ins_End:
            tracks.push_back(move(t));
        }
    }

    return true;
}

