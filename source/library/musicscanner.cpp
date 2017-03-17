#include "includes/library/musicscanner.hpp"

MusicScanner::MusicScanner(QFileInfoList &musicFiles)
        : files(musicFiles)
{

}

void MusicScanner::run()
{
        QList<Song> songs;
        qRegisterMetaType<QList<Song>>("QList<Song>");
        for (auto &file : files) {
                // TODO: Think about whether to emit for each song, or at the end
                songs.append(Song(file));
        }

        emit passNewItems(songs);
}
