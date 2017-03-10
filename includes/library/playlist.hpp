#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <QMediaPlaylist>

class Playlist : public QMediaPlaylist
{
Q_OBJECT

public:
    Playlist();

    void next();
    void previous();

private:

};


#endif //PLAYLIST_HPP
