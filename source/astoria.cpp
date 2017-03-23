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
TagLib::FileRef Astoria::getCurrentTag()
{
        return TagLib::FileRef(getCurrentSong().toString().remove(0, 7).toStdString().c_str());
}

QUrl Astoria::getCurrentSong()
{
        return getAudioInstance()->currentMedia().canonicalUrl();
}
