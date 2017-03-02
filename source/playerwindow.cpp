#include "includes/playerwindow.hpp"
#include "ui_playerwindow.h"

#include <QMediaMetaData>

#include "includes/playercontrols.hpp"
#include "includes/volumecontrols.hpp"
#include "includes/durationcontrols.hpp"
#include "includes/menubar.hpp"

/**
 * To add features list
 *	- Allow the stylesheet to be read from a file (.qss file)
 *	- User adjustable previous song time limit (that is, how long into a song does pressing
 *	  the previous button restart the song)
 *		- Maybe have an 'always' setting
 */

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);

    //setWindowTitle(Project);
    setStyleSheet("background-color: #222222;");

    player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("/home/pyxxil/Downloads/01 - Welcome to the Black Parade.mp3"));

    PlayerControls *playerControls = new PlayerControls(this);
    playerControls->setState(player->state());

    connect(playerControls, SIGNAL(play()),
            player, SLOT(play()));
    connect(playerControls, SIGNAL(pause()),
            player, SLOT(pause()));
    connect(playerControls, SIGNAL(next()),
            this, SLOT(nextSong()));
    connect(playerControls, SIGNAL(previous()),
            this, SLOT(previousSong()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            playerControls, SLOT(setState(QMediaPlayer::State)));

    VolumeControls *volumeControls = new VolumeControls(this);
    volumeControls->setVolume(player->volume());
    volumeControls->setMute(player->isMuted());
    volumeControls->setMinimumSize(200, 0);
    volumeControls->setMaximumSize(200, volumeControls->height());

    connect(volumeControls, SIGNAL(changeVolume(int)),
            player, SLOT(setVolume(int)));
    connect(volumeControls, SIGNAL(mute(bool)),
            player, SLOT(setMuted(bool)));
    connect(player, SIGNAL(volumeChanged(int)),
            volumeControls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)),
            volumeControls, SLOT(setMute(bool)));

    DurationControls *durationControls = new DurationControls(this);
    connect(player, SIGNAL(positionChanged(qint64)),
            durationControls, SLOT(positionChanged(qint64)));
    connect(durationControls, SIGNAL(seek(int)),
            this, SLOT(timeSeek(int)));

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(mediaLoaded(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(currentMediaChanged(const QMediaContent &)),
            this, SLOT(songChanged(const QMediaContent &)));
    connect(this, SIGNAL(durationChanged(qint64)),
            durationControls, SLOT(songChanged(qint64)));

    menu = new MenuBar(this, playerControls);
    for (auto &_menu : menu->getAllMenus()) {
        ui->menuBar->addMenu(_menu);
    }

    QHBoxLayout *controlLayout = new QHBoxLayout(this);
    controlLayout->addWidget(playerControls);
    controlLayout->addWidget(volumeControls);
    controlLayout->addWidget(durationControls);

    ui->controlsWidget->setLayout(controlLayout);
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}

void PlayerWindow::nextSong()
{
    // Next song in playlist
}

/**
 * Go to the previous song, unless the time played into the current song is less than 10 seconds.
 *
 * @brief PlayerWindow::previousSong
 */
void PlayerWindow::previousSong()
{
    // TODO: Make the time to go to the previous song adjustable
    if (player->position() < 10000) {
        player->setPosition(0);
    }
    else {
        // Previous song in playlist
    }
}

QMediaPlayer::State PlayerWindow::playerState() const
{
    return player->state();
}

void PlayerWindow::timeSeek(int time)
{
    player->setPosition(time);
}

void PlayerWindow::songChanged(const QMediaContent &)
{
    emit durationChanged(player->duration());
    setWindowTitle(QString("%1 - %2")
                       .arg(player->metaData(QMediaMetaData::AlbumArtist).toString())
                       .arg(player->metaData(QMediaMetaData::Title).toString()));
}

void PlayerWindow::mediaLoaded(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia) {
        emit durationChanged(player->duration());
        setWindowTitle(QString("%1 - %2")
                           .arg(player->metaData(QMediaMetaData::Author).toString())
                           .arg(player->metaData(QMediaMetaData::Title).toString()));
    }
}
