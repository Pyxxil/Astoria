#ifndef SONG_HPP
#define SONG_HPP

// Taglib, at least on OSX, throws a couple of deprecated declaration warnings
// which are annoying to see, and interfere with -Werror. This might not be a
// good thing to do, but it solves this problem for now.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <fileref.h>
#pragma GCC diagnostic pop

#include <QString>
#include <QMap>
#include <QFileInfo>

class Song
{
public:
        Song(const QFileInfo &);
        Song(const Song &other);

        ~Song() = default;

        const QMap<QString, QString> &getMetadata() const;

        QString filePath;
        TagLib::FileRef file;

        void updateMetadata();

private:
        QMap<QString, QString> metadata;
};

#endif //SONG_HPP
