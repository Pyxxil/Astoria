#ifndef MUSICPLAYER_GLOBALS_HPP
#define MUSICPLAYER_GLOBALS_HPP

class QMediaPlayer;

namespace Globals {
        void init();

        QMediaPlayer &getAudioInstance();
}

#endif //MUSICPLAYER_GLOBALS_HPP
