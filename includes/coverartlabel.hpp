#ifndef ASTORIA_COVERART_HPP
#define ASTORIA_COVERART_HPP

#include <QLabel>
// Taglib, at least on OSX, throws a couple of deprecated declaration warnings
// which are annoying to see, and interfere with -Werror. This might not be a
// good thing to do, but it solves this problem for now.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include "taglib.h"
#include "fileref.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

class CoverArtLabel : public QLabel
{
Q_OBJECT

public:
        CoverArtLabel(QWidget *parent = nullptr);

public slots:
        void artChanged(TagLib::FileRef newSong);
        // void songLoaded();  For later, when setting up loading of songs

private:
        QImage *coverArtImage = nullptr;
};

#endif //ASTORIA_COVERART_HPP
