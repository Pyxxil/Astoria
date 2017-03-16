#ifndef GLOBALS_HPP
#define GLOBALS_HPP

class QMediaPlayer;
class QMediaPlaylist;

namespace Globals
{
        void init();
        void deInit();

        QMediaPlayer *getAudioInstance();
        QMediaPlaylist *getPlaylistInstance();
}

#endif // GLOBALS_HPP
