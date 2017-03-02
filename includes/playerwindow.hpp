#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QToolButton>

class MenuBar;

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
    void songChanged(const QMediaContent &);
    void mediaLoaded(QMediaPlayer::MediaStatus);

private:
    Ui::PlayerWindow *ui;
    QMediaPlayer *player;
    MenuBar *menu;
};

#endif // PLAYERWINDOW_H
