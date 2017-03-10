#include "includes/library/song.hpp"

#include <iostream>
#include <iomanip>
#include <id3v2tag.h>
#include <id3v2extendedheader.h>
#include <tpropertymap.h>s

Song::Song(const QFileInfo &filePath)
    : filePath(filePath.absoluteFilePath())
{
    file = TagLib::FileRef(this->filePath.toStdString().c_str());

    if (!file.isNull() && file.tag()) {

        // TODO: Use this stuff (Taglib example tagreader), as well as audioProperties
        TagLib::PropertyMap tags = file.file()->properties();

        for (TagLib::PropertyMap::ConstIterator it = tags.begin(); it != tags.end(); ++it) {
            for(TagLib::StringList::ConstIterator j = it->second.begin(); j != it->second.end(); ++j) {
                std::cout << std::left << std::setw(10) << it->first << " - " << '"' << *j << '"' << std::endl;
            }
        }

        metadata = {
            {"Title", TStringToQString(file.tag()->title())},
            {"Artist", TStringToQString(file.tag()->artist())},
            {"Album", TStringToQString(file.tag()->album())},
            {"Year", QString("%1").arg(file.tag()->year())},
            {"Track", QString("%1").arg(file.tag()->track())},
            {"Genre", TStringToQString(file.tag()->genre())}
        };
    }
}

const QMap<QString, QString> &Song::metaData() const
{
    return metadata;
}
