#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <fileref.h>

class TrackInformation;
class DurationControls;
class RightClickMenu;
class VolumeControls;
class PlayerControls;
class LibraryModel;
class QTableView;
class MenuBar;
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
    QTableView *libraryView;

    QImage image;

    void setupConnections();
    void setupUI();
    void loadCoverArt(TagLib::FileRef &song);
};

#endif // PLAYERWINDOW_H
