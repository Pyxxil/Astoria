#include "includes/musicscanner.hpp"

#include <QMediaPlayer>
#include <QEventLoop>
#include <QMediaMetaData>

MusicScanner::MusicScanner(QMap<QString, QMap<QString, QString>> *lib, QFileInfoList &musicFiles)
    : library(lib)
      , files(musicFiles)
{

}

void MusicScanner::run()
{
    QMediaPlayer player;
    QEventLoop loop;
    for (const auto &file : files) {
        player.setMedia(QUrl::fromLocalFile(file.absoluteFilePath()));
        loop.connect(&player, SIGNAL(metaDataChanged()), SLOT(quit()));
        loop.exec();
        library->insert(file.absoluteFilePath(), {
            { "Author", player.metaData(QMediaMetaData::Author).toString() },
            { "Title",  player.metaData(QMediaMetaData::Title).toString() },
            { "Artist", player.metaData(QMediaMetaData::ContributingArtist).toString() },
            { "Album",  player.metaData(QMediaMetaData::AlbumTitle).toString() },
        });
    }
}

void MusicScanner::setFiles(QFileInfoList &musicFiles)
{
    files = musicFiles;
}
