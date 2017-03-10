#include "includes/library/playlist.hpp"

Playlist::Playlist()
    : QMediaPlaylist()
{

}

void Playlist::next()
{
    QMediaPlaylist::next();
}

void Playlist::previous()
{
    QMediaPlaylist::previous();
}
