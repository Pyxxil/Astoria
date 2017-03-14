#ifndef MUSICSCANNER_H
#define MUSICSCANNER_H

#include <QFileInfoList>
#include <QThread>
#include <QList>

#include "includes/library/song.hpp"

class MusicScanner : public QThread
{
Q_OBJECT

signals:
        void passNewItems(QList<Song>);

public:
        MusicScanner(QFileInfoList &);

        void run() Q_DECL_OVERRIDE;

private:
        QFileInfoList files;
};

#endif //MUSICSCANNER_H
