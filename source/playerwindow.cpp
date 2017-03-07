#include "includes/playerwindow.hpp"
#include "ui_playerwindow.h"

#include <QMediaMetaData>
#include <QDockWidget>

#include "includes/playercontrols.hpp"
#include "includes/volumecontrols.hpp"
#include "includes/durationcontrols.hpp"
#include "includes/librarymodel.hpp"
#include "includes/menubar.hpp"

/**
 * To add features list
 *	- Allow the stylesheet to be read from a file (.qss file)
 *	- User adjustable previous song time limit (that is, how long into a song does pressing
 *	  the previous button restart the song)
 *	- Allow the user to remove/add headers and change the order of them
 *	    - These can be picked from whatever metadata can be grabbed
 */

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PlayerWindow), lastPreviousClick(QTime::currentTime())
{
    ui->setupUi(this);

    //setWindowTitle(Project);
    //setStyleSheet("background-color: #222222;");

    player = new QMediaPlayer(this);

    PlayerControls *playerControls = new PlayerControls(ui->controlsWidget);
    playerControls->setState(player->state());
    playerControls->setMinimumWidth(200);
    playerControls->setMaximumWidth(200);

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

    VolumeControls *volumeControls = new VolumeControls(ui->controlsWidget);
    volumeControls->setVolume(player->volume());
    volumeControls->setMute(player->isMuted());
    volumeControls->setMinimumSize(200, playerControls->height());
    volumeControls->setMaximumSize(200, playerControls->height());

    connect(volumeControls, SIGNAL(changeVolume(int)),
            player, SLOT(setVolume(int)));
    connect(volumeControls, SIGNAL(mute(bool)),
            player, SLOT(setMuted(bool)));
    connect(player, SIGNAL(volumeChanged(int)),
            volumeControls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)),
            volumeControls, SLOT(setMute(bool)));

    DurationControls *durationControls = new DurationControls(ui->durationWidget);
    durationControls->setMinimumSize(200, playerControls->height());
    connect(player, SIGNAL(positionChanged(qint64)),
            durationControls, SLOT(positionChanged(qint64)));
    connect(durationControls, SIGNAL(seek(int)),
            this, SLOT(timeSeek(int)));

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(mediaLoaded(QMediaPlayer::MediaStatus)));
    connect(this, SIGNAL(durationChanged(qint64)),
            durationControls, SLOT(songChanged(qint64)));
    connect(player, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));

    library= new LibraryModel();
    ui->libraryView->setModel(library);
    connect(library, SIGNAL(libraryUpdated(int, int)), ui->libraryView, SLOT(rowCountChanged(int, int)));

    menu = new MenuBar();
    for (auto &_menu : menu->getAllMenus()) {
        ui->menuBar->addMenu(_menu);
    }

    connect(menu, SIGNAL(play()), playerControls, SIGNAL(play()));
    connect(menu, SIGNAL(pause()), playerControls, SIGNAL(pause()));
    connect(menu, SIGNAL(gotoNextSong()), this, SLOT(nextSong()));
    connect(menu, SIGNAL(gotoPreviousSong()), this, SLOT(previousSong()));
    connect(menu, SIGNAL(updateLibrary()), library, SLOT(openDirectory()));

    player->setPlaylist(library->playlist);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addStretch(1);
    controlLayout->addWidget(playerControls);
    controlLayout->addStretch(1);
    controlLayout->addWidget(volumeControls);

    ui->controlsWidget->setLayout(controlLayout);

    ui->durationWidget->setLayout(new QVBoxLayout());
    ui->durationWidget->layout()->addWidget(durationControls);
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}

void PlayerWindow::nextSong()
{
    player->playlist()->next();
}

/**
 * Go to the previous song, unless the time played into the current song is less than 10 seconds.
 *
 * @brief PlayerWindow::previousSong
 */
void PlayerWindow::previousSong()
{
    // TODO: Make the time to go to the previous song adjustable
    // TODO: Remove this
    qDebug() << lastPreviousClick.msecsTo(QTime::currentTime());
    if (player->position() < 10000 || lastPreviousClick.msecsTo(QTime::currentTime()) < 1000) {
        player->playlist()->previous();
    }
    else {
        lastPreviousClick = QTime::currentTime();
        player->setPosition(0);
    }
}

QMediaPlayer::State PlayerWindow::playerState() const
{
    if (player == nullptr) { return QMediaPlayer::StoppedState; }
    else { return player->state(); }
}

void PlayerWindow::timeSeek(int time)
{
    player->setPosition(time);
}

void PlayerWindow::mediaLoaded(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia) {
        emit durationChanged(player->duration());
        setWindowTitle(QString("%1 - %2")
                           .arg(player->metaData(QMediaMetaData::ContributingArtist).toString())
                           .arg(player->metaData(QMediaMetaData::Title).toString()));
    }
}

void PlayerWindow::metaDataChanged()
{
    emit durationChanged(player->duration());
    setWindowTitle(QString("%1 - %2")
                       .arg(player->metaData(QMediaMetaData::ContributingArtist).toString())
                       .arg(player->metaData(QMediaMetaData::Title).toString()));
}
