#ifndef SONG_HPP
#define SONG_HPP

#include <fileref.h>

#include <QString>
#include <QMap>
#include <QFileInfo>

class Song
{

public:
    Song(const QFileInfo &);

    const QMap<QString, QString> &metaData() const;

    QString filePath;

private:
    TagLib::FileRef file;
    QMap<QString, QString> metadata;
};


#endif //SONG_HPP
