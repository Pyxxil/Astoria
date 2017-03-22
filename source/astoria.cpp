#include "includes/astoria.hpp"

#include <QMediaPlayer>
#include <QMediaPlaylist>

void Astoria::init()
{
        Audio::init();
        Playlist::init();
        UI::init();
}

void Astoria::deInit()
{
        Audio::deInit();
        Playlist::deInit();
        UI::deInit();
}

QMediaPlayer *Astoria::getAudioInstance()
{
        return Audio::player;
}

QMediaPlaylist *Astoria::getPlaylistInstance()
{
        return Playlist::playlist;
}

QUrl Astoria::getCurrentSong()
{
        return getAudioInstance()->currentMedia().canonicalUrl();
}
