#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QToolButton>
#include <QTime>
#include <QGraphicsScene>

class MenuBar;
class LibraryModel;
class RightClickMenu;
class DurationControls;
class VolumeControls;
class PlayerControls;
class TrackInformation;
class QLabel;

namespace Ui
{
class PlayerWindow;
}

class PlayerWindow: public QMainWindow
{
Q_OBJECT

signals:
    void durationChanged(qint64);
    void informationChanged(QString, QString);

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

    QMediaPlayer::State playerState() const;

public slots:
    void nextSong();
    void previousSong();
    void timeSeek(int);
    void metaDataChanged();
    void playNow();
    void customMenuRequested(QPoint pos);
    void updatePlaylist();
    void play();

private:
    Ui::PlayerWindow *ui;
    QMediaPlayer *player;
    LibraryModel *library;
    DurationControls *durationControls;
    VolumeControls *volumeControls;
    PlayerControls *playerControls;
    TrackInformation *information;
    MenuBar *menu;
    RightClickMenu *rightClickMenu;

    QLabel *coverArtLabel;

    QImage image;

    void setUpConnections();
    void setUpLayouts();
};

#endif // PLAYERWINDOW_H
