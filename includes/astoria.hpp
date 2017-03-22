#ifndef ASTORIA_NAMESPACE_HPP
#define ASTORIA_NAMESPACE_HPP

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
}

#endif // ASTORIA_NAMESPACE_HPP
