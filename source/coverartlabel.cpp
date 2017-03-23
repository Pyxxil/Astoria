#include "includes/coverartlabel.hpp"

#include <QMimeDatabase>

// Taglib, at least on OSX, throws a couple of deprecated declaration warnings
// which are annoying to see, and interfere with -Werror. This might not be a
// good thing to do, but it solves this problem for now.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "mpegfile.h"
#include "attachedpictureframe.h"
#include "id3v2tag.h"
#include "id3v2extendedheader.h"
#include "mp4tag.h"
#include "mp4file.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

#include "includes/astoria.hpp"

CoverArtLabel::CoverArtLabel(QWidget *parent)
        : QLabel(parent),
          coverArtImage(new QImage)
{
        setScaledContents(true);
        setBackgroundRole(QPalette::Base);
        setContentsMargins(10, 0, 0, 0);
        setMaximumSize(200, 200);
        setMinimumSize(200, 200);
        setAlignment(Qt::AlignCenter);

        coverArtImage->load(":/assets/CoverArtUnavailable.png");
        setPixmap(QPixmap::fromImage(*coverArtImage));
}

/**
 * When the song changes, it can (quite often, safely) be assumed that the artwork has
 * also changed.
 *
 * This shouldn't be much of a performance hit even though it's called every time the
 * song is changed, and would still reload if the album art we want to change to is the
 * same as the one currently being displayed.
 */
void CoverArtLabel::artChanged(TagLib::FileRef newSong)
{
        QMimeDatabase db;
        QMimeType codec = db.mimeTypeForFile(newSong.file()->name());

        bool coverArtNotFound = true;

        if (codec.name() == "audio/mp4") {
                TagLib::MP4::File mp4(newSong.file()->name());
                if (mp4.tag() && mp4.tag()->itemListMap().contains("covr")) {
                        TagLib::MP4::CoverArtList coverArtList =
                                mp4.tag()->itemListMap()["covr"].toCoverArtList();

                        if (!coverArtList.isEmpty()) {
                                TagLib::MP4::CoverArt coverArt = coverArtList.front();
                                coverArtImage->loadFromData(
                                        reinterpret_cast<const uchar *>(coverArt.data().data()),
                                        static_cast<int>(coverArt.data().size()));
                                coverArtNotFound = false;
                        }
                }
        } else if (codec.name() == "audio/mpeg") {
                TagLib::MPEG::File file(newSong.file()->name());
                if (file.ID3v2Tag()) {
                        TagLib::ID3v2::FrameList frameList = file.ID3v2Tag()->frameList("APIC");

                        if (!frameList.isEmpty()) {
                                TagLib::ID3v2::AttachedPictureFrame *coverImg =
                                        static_cast<
                                                TagLib::ID3v2::AttachedPictureFrame *
                                                >
                                        (frameList.front());

                                coverArtImage->loadFromData(
                                        reinterpret_cast<const uchar *>(coverImg->picture().data()),
                                        static_cast<int>(coverImg->picture().size()));
                                coverArtNotFound = false;
                        }
                }
        }

        if (coverArtNotFound) {
                coverArtImage->load(":/assets/CoverArtUnavailable.png");
        }

        setPixmap(QPixmap::fromImage(*coverArtImage));
}
