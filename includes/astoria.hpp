#ifndef ASTORIA_NAMESPACE_HPP
#define ASTORIA_NAMESPACE_HPP

// Taglib, at least on OSX, throws a couple of deprecated declaration warnings
// which are annoying to see, and interfere with -Werror. This might not be a
// good thing to do, but it solves this problem for now.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include "taglib.h"
#include "fileref.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

class QMediaPlayer;
class QMediaPlaylist;
class QUrl;

namespace Astoria
{
        namespace Audio
        {
                void init();
                void deInit();

                extern QMediaPlayer *player;
        }

        namespace Playlist
        {
                void init();
                void deInit();

                extern QMediaPlaylist *playlist;
        }

        namespace UI
        {
                void init();
                void deInit();
        }

        void init();
        void deInit();

        QMediaPlayer *getAudioInstance();
        QMediaPlaylist *getPlaylistInstance();

        QUrl getCurrentSong();
        TagLib::FileRef getCurrentTag();
}

#endif // ASTORIA_NAMESPACE_HPP
