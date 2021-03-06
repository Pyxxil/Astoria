#ifndef TRACKINFORMATION_HPP
#define TRACKINFORMATION_HPP

#include <QWidget>
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

class TrackInformation : public QWidget
{
Q_OBJECT

public:
        TrackInformation(QWidget *parent = nullptr, int minWidth = 16777215, int maxWidth = 16777215);

public slots:
        void updateLabels(TagLib::FileRef);

private:
        QLabel *songLabel;
        QLabel *artistLabel;
};

#endif // TRACKINFORMATION_HPP
