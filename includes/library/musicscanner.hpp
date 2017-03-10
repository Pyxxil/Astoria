#ifndef MUSICSCANNER_H
#define MUSICSCANNER_H

#include <QFileInfoList>
#include <QThread>
#include <QList>

#include "includes/library/song.hpp"

class MusicScanner: public QThread
{
Q_OBJECT

public:
    MusicScanner(QList<Song> &, QFileInfoList &);

    void run() Q_DECL_OVERRIDE;

private:
    QList<Song> &library;
    QFileInfoList files;
};


#endif //MUSICSCANNER_H
