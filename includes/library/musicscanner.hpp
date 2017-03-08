#ifndef MUSICPLAYER_MUSICSCANNER_H
#define MUSICPLAYER_MUSICSCANNER_H

#include <QThread>
#include <QFileInfoList>

class MusicScanner : public QThread
{
    Q_OBJECT

public:
    MusicScanner(QMap<QUrl, QMap<QString, QString>> *, QFileInfoList &);

    void run();
    void setFiles(QFileInfoList &);

private:
    QMap<QUrl, QMap<QString, QString>> *library;
    QFileInfoList files;

};


#endif //MUSICPLAYER_MUSICSCANNER_H
