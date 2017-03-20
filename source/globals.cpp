#include "includes/globals.hpp"

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>

/*
 * TODO: Seperate this into multiple files, e.g.
 *      Project Root Directory
 *              | - includes
 *              |       | - controls
 *              |       | - delegates
 *              |       | - globals  -- This will be changed to the name of the project
 *              |       |       | - audio
 *              |       |       | - playlist
 *              |       |       | - ui
 *              |       |       | ...
 *              |       | - library
 *              |       | - menus
 *              |       | ...
 *              | - source
 *              |       | - controls
 *              |       | - delegates
 *              |       | - globals  -- This will be changed to the name of the project
 *              |       |       | - audio
 *              |       |       | - playlist
 *              |       |       | - ui
 *              |       |       | ...
 *              |       | - library
 *              |       | ...
 *              | ...
 *
 * TODO: Come up with a name for the project, for a few reasons, one being so this file,
 * TODO: and all related files can actually belong to a namespace with the name of the
 * TODO: project, and not 'Globals'.
 */

namespace Globals
{
        namespace Audio
        {
                void init();
                void deInit();

                QMediaPlayer *player;
        }

        namespace Playlist
        {
                void init();
                void deInit();

                QMediaPlaylist *playlist;
        }

        namespace UI
        {
                void init();
                void deInit();
        }
}

void Globals::init()
{
        Audio::init();
        Playlist::init();
        UI::init();
}

void Globals::deInit()
{
        Audio::deInit();
        Playlist::deInit();
        UI::deInit();
}

void Globals::Audio::init()
{
        player = new QMediaPlayer;
}

void Globals::Audio::deInit()
{

}

void Globals::Playlist::init()
{
        playlist = new QMediaPlaylist;
        Audio::player->setPlaylist(playlist);
}

void Globals::Playlist::deInit()
{
}

void Globals::UI::init()
{

}

void Globals::UI::deInit()
{

}

QMediaPlayer *Globals::getAudioInstance()
{
        return Audio::player;
}

QMediaPlaylist *Globals::getPlaylistInstance()
{
        return Playlist::playlist;
}

QUrl Globals::getCurrentSong()
{
        return getAudioInstance()->currentMedia().canonicalUrl();
}
