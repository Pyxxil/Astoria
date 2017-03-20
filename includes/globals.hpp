#ifndef GLOBALS_HPP
#define GLOBALS_HPP

class QMediaPlayer;
class QMediaPlaylist;
class QUrl;

namespace Globals
{
        void init();
        void deInit();

        QMediaPlayer *getAudioInstance();
        QMediaPlaylist *getPlaylistInstance();

        QUrl getCurrentSong();
}

#endif // GLOBALS_HPP
