#include "includes/library/musicscanner.hpp"

#include <QMediaPlayer>
#include <QEventLoop>
#include <QMediaMetaData>

MusicScanner::MusicScanner(QMap<QUrl, QMap<QString, QString>> *lib, QFileInfoList &musicFiles)
    : library(lib)
      , files(musicFiles)
{

}

void MusicScanner::run()
{
    QMediaPlayer player;
    QEventLoop loop;
    for (const auto &file : files) {
        QUrl filePath = QUrl::fromLocalFile(file.absoluteFilePath());
        if (library->count(filePath) == 0) {
            player.setMedia(filePath);
            loop.connect(&player, SIGNAL(metaDataChanged()), SLOT(quit()));
            loop.exec();
            library->insert(filePath, {
                {"Author", player.metaData(QMediaMetaData::Author).toString()},
                {"Title", player.metaData(QMediaMetaData::Title).toString()},
                {"Artist", player.metaData(QMediaMetaData::ContributingArtist).toString()},
                {"Album", player.metaData(QMediaMetaData::AlbumTitle).toString()},
            });
        }
    }
}

void MusicScanner::setFiles(QFileInfoList &musicFiles)
{
    files = musicFiles;
}
