#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QToolButton>
#include <QTime>

class MenuBar;
class LibraryModel;

namespace Ui
{
class PlayerWindow;
}

class PlayerWindow: public QMainWindow
{
Q_OBJECT

signals:
    void durationChanged(qint64);

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

    QMediaPlayer::State playerState() const;

public slots:
    void nextSong();
    void previousSong();
    void timeSeek(int);
    void mediaLoaded(QMediaPlayer::MediaStatus);
    void metaDataChanged();

private:
    Ui::PlayerWindow *ui;
    QMediaPlayer *player;
    LibraryModel *library;
    MenuBar *menu;
    QTime lastPreviousClick;
};

#endif // PLAYERWINDOW_H
