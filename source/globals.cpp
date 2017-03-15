#include "includes/globals.hpp"

#include <QMediaPlayer>

namespace Globals {
        namespace Audio
        {
                QMediaPlayer *player;
        }

        namespace Playlist
        {

        }

        namespace UI
        {

        }
}

void Globals::init()
{
        Audio::player = new QMediaPlayer;
}

QMediaPlayer &Globals::getAudioInstance()
{
        return *Globals::Audio::player;
}