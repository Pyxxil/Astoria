#include "includes/playerwindow.hpp"
#include "ui_playerwindow.h"

#include "includes/playercontrols.hpp"
#include "includes/volumecontrols.hpp"
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
    setStyleSheet("background-color: #222222;");

    player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("/home/pyxxil/Downloads/02. The Writer (Album Version).mp3"));

    PlayerControls *playerControls = new PlayerControls(this);
    playerControls->setState(player->state());

    connect(playerControls, SIGNAL(play()), player, SLOT(play()));
    connect(playerControls, SIGNAL(pause()), player, SLOT(pause()));
    connect(playerControls, SIGNAL(next()), this, SLOT(nextSong()));
    connect(playerControls, SIGNAL(previous()), this, SLOT(previousSong()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), playerControls, SLOT(setState(QMediaPlayer::State)));

    VolumeControls *volumeControls = new VolumeControls(this);
    volumeControls->setVolume(player->volume());
    volumeControls->setMute(player->isMuted());

    connect(volumeControls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(volumeControls, SIGNAL(mute(bool)), player, SLOT(setMuted(bool)));
    connect(player, SIGNAL(volumeChanged(int)), volumeControls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), volumeControls, SLOT(setMute(bool)));

    menu = new MenuBar(this, playerControls);

    QHBoxLayout *controlLayout = new QHBoxLayout(this);
    controlLayout->setContentsMargins(40, 0, 40, 0);
    controlLayout->addWidget(playerControls);
    controlLayout->addWidget(volumeControls);

    ui->controlsWidget->setLayout(controlLayout);

    for (auto &_menu : menu->getAllMenus()) {
        ui->menuBar->addMenu(_menu);
    }
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
