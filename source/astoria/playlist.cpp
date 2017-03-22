#include "includes/astoria.hpp"

#include <QMediaPlaylist>
#include <QMediaPlayer>

namespace Astoria
{
        namespace Playlist
        {
                QMediaPlaylist *playlist;
        }
}

void Astoria::Playlist::init()
{
        Astoria::Playlist::playlist = new QMediaPlaylist;
        Astoria::Audio::player->setPlaylist(playlist);
}

void Astoria::Playlist::deInit()
{

}
