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

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

    QMediaPlayer::State playerState() const;

public slots:
    void nextSong();
    void previousSong();

private:
    Ui::PlayerWindow *ui;
    QMediaPlayer *player;
    MenuBar *menu;
};

#endif // PLAYERWINDOW_H
