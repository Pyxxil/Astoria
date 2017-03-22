#include "includes/astoria.hpp"

#include <QMediaPlayer>

namespace Astoria
{
        namespace Audio
        {
                QMediaPlayer *player;
        }
}

void Astoria::Audio::init()
{
        Astoria::Audio::player = new QMediaPlayer;
}

void Astoria::Audio::deInit()
{

}
