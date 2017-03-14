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
        /*
        bool isAlreadyInLibrary = false;
        // TODO: Think about if this is the right choice. With a large library,
        // this could take a lot longer than just adding the song.. But otherwise,
        // we're adding the same song multiple times. Probably best to change the
        // underlying structure to a QMap<QUrl|QString, Song>.
        for (const auto &song : library) {
            if (song.filePath == file.absoluteFilePath()) {
                isAlreadyInLibrary = true;
                break;
            }
        }
        */

        songs.append(Song(file));
    }

    emit passNewItems(songs);
}
