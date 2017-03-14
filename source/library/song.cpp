#include "includes/library/song.hpp"

// Taglib, at least on OSX, throws a couple of deprecated declaration warnings
// which are annoying to see, and interfere with -Werror. This might not be a
// good thing to do, but it solves this problem for now.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <id3v2tag.h>
#include <id3v2extendedheader.h>
#pragma GCC diagnostic pop

#include <QDebug>

Song::Song(const QFileInfo &filePath)
    : filePath(filePath.absoluteFilePath())
{
    file = TagLib::FileRef(this->filePath.toStdString().c_str());

    updateMetadata();
}

Song::Song(const Song &other)
    : filePath(other.filePath)
{
    file = TagLib::FileRef(other.filePath.toStdString().c_str());

    updateMetadata();
}

const QMap<QString, QString> &Song::getMetadata() const
{
    return metadata;
}

void Song::updateMetadata()
{
    if (!file.isNull() && file.tag()) {

        // TODO: Use this stuff (Taglib example tagreader), as well as audioProperties
        // TODO: Probably use it for editing tags and such later.
        /*
        TagLib::PropertyMap tags = file.file()->properties();

        for (TagLib::PropertyMap::ConstIterator it = tags.begin(); it != tags.end(); ++it) {
            for(TagLib::StringList::ConstIterator j = it->second.begin(); j != it->second.end(); ++j) {
                std::cout << std::left << std::setw(10) << it->first << " - " << '"' << *j << '"' << std::endl;
            }
        }
        */

        metadata = {
            {"Title", TStringToQString(file.tag()->title())},
            {"Artist", TStringToQString(file.tag()->artist())},
            {"Album", TStringToQString(file.tag()->album())},
            {"Year", QString("%1").arg(file.tag()->year())},
            {"Track", QString("%1").arg(file.tag()->track())},
            {"Genre", TStringToQString(file.tag()->genre())},
            // TODO: Should probably change this part, what if it can't find the audio properties?
            {"Duration", QString("%1:%2")
                .arg(file.audioProperties()->length() / 60, 2, 10, QChar('0'))
                .arg(file.audioProperties()->length() % 60, 2, 10, QChar('0'))
            },
        };
    }
}
