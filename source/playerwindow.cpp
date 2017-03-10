#include "includes/playerwindow.hpp"
#include "ui_playerwindow.h"

#include <QMediaMetaData>
#include <QHBoxLayout>
#include <QLabel>

#include "includes/controls/playercontrols.hpp"
#include "includes/controls/volumecontrols.hpp"
#include "includes/controls/durationcontrols.hpp"
#include "includes/library/librarymodel.hpp"
#include "includes/menus/menubar.hpp"
#include "includes/menus/rightclickmenu.hpp"
#include "includes/trackinformation.hpp"

#include <mpegfile.h>
#include <attachedpictureframe.h>
#include <id3v2tag.h>
#include <id3v2extendedheader.h>
#include <id3v2frame.h>

/**
 * TODO: Possible features
 * To add features list
 *	- Allow the stylesheet to be read from a file (.qss file)
 *	- User adjustable previous song time limit (that is, how long into a song does pressing
 *	  the previous button restart the song)
 *	- Allow the user to remove/add headers and change the order of them
 *	    - These can be picked from whatever metadata can be grabbed
 *	- Saveability
 *	    - Save state
 *	    - Save library
 *	    - Save playlists
 *	- Alter file metadata
 *
 * TODO: Fixes
 *  - Fix up UI stuff
 *  - Add other tags to columns in the library view
 *  - Fix the Playlist problems (playing next, previous crashes, change playlist set-up)
 */

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);

    //setWindowTitle(Project);
    setStyleSheet("background-color: #333333;");

    player = new QMediaPlayer(this);

    playerControls = new PlayerControls(this, player->state());
    volumeControls = new VolumeControls(this, player->volume(),
                                        player->isMuted(), 150, 150);
    volumeControls->setContentsMargins(0, 0, 5, 0);
    durationControls = new DurationControls(this, 200);

    library = new LibraryModel();

    ui->libraryView->setModel(library);
    ui->libraryView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    player->setPlaylist(library->playlist);

    menu = new MenuBar();
    for (auto &_menu : menu->getAllMenus()) {
        ui->menuBar->addMenu(_menu);
    }

    information = new TrackInformation();

    rightClickMenu = new RightClickMenu(this);

    coverArtLabel = new QLabel(this);
    coverArtLabel->setScaledContents(true);
    coverArtLabel->setBackgroundRole(QPalette::Base);
    coverArtLabel->setMinimumSize(200, 200);
    coverArtLabel->setMaximumSize(200, 200);
    coverArtLabel->setContentsMargins(0, 0, 0, 0);

    setUpConnections();
    setUpLayouts();
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}

void PlayerWindow::nextSong()
{
    if (!player->playlist()->isEmpty()) {
        player->playlist()->next();
    }
}

/**
 * Go to the previous song, unless the time played into the current song is less than 10 seconds.
 */
void PlayerWindow::previousSong()
{
    // TODO: Make the time to go to the previous song adjustable
    // TODO: What to do if the user has pressed go to previous and there aren't any songs
    // TODO: before it? Do we set position to 0, and pause the media? Or just reset the song?
    if (player->position() < 10000 && player->playlist()->currentIndex() > 0) {
        player->playlist()->previous();
    }
    else {
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

void PlayerWindow::metaDataChanged()
{
    emit durationChanged(player->duration());
    TagLib::FileRef song(player->currentMedia().canonicalUrl().toString().remove(0, 7).toStdString().c_str());
    setWindowTitle(QString("%1 - %2")
                       .arg(TStringToQString(song.tag()->artist()))
                       .arg(TStringToQString(song.tag()->title())));
    emit informationChanged(TStringToQString(song.tag()->artist()),
                            TStringToQString(song.tag()->title()));

    TagLib::MPEG::File file(song.file()->name());
    TagLib::ID3v2::Tag *m_tag = file.ID3v2Tag(true);
    TagLib::ID3v2::FrameList frameList = m_tag->frameList("APIC");
    if(frameList.isEmpty()) {
        qDebug() << "Couldn't find an image for that file.";
        return;
    }

    TagLib::ID3v2::AttachedPictureFrame *coverImg = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());

    image.loadFromData((const uchar *) coverImg->picture().data(), coverImg->picture().size());
    coverArtLabel->setPixmap(QPixmap::fromImage(image));
}

void PlayerWindow::play()
{
    if (!library->playlist->isEmpty()) {
        emit player->play();
    }
}

void PlayerWindow::playNow()
{
    // TODO: Fix this.
    player->playlist()->insertMedia(0, library->get(ui->libraryView->currentIndex().row()));
    player->playlist()->setCurrentIndex(0);
    emit player->play();
}

void PlayerWindow::customMenuRequested(QPoint pos)
{
    if (ui->libraryView->indexAt(pos).isValid()) {
        emit rightClickMenu->display(ui->libraryView->viewport()->mapToGlobal(pos));
    }
}

void PlayerWindow::updatePlaylist()
{
    // TODO: Figure out what to do here
}

void PlayerWindow::setUpConnections()
{
    connect(playerControls, SIGNAL(play()),
            this, SLOT(play()));
    connect(playerControls, SIGNAL(pause()),
            player, SLOT(pause()));
    connect(playerControls, SIGNAL(next()),
            this, SLOT(nextSong()));
    connect(playerControls, SIGNAL(previous()),
            this, SLOT(previousSong()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            playerControls, SLOT(setState(QMediaPlayer::State)));

    connect(volumeControls, SIGNAL(changeVolume(int)),
            player, SLOT(setVolume(int)));
    connect(volumeControls, SIGNAL(mute(bool)),
            player, SLOT(setMuted(bool)));
    connect(player, SIGNAL(volumeChanged(int)),
            volumeControls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)),
            volumeControls, SLOT(setMute(bool)));

    connect(player, SIGNAL(positionChanged(qint64)),
            durationControls, SLOT(positionChanged(qint64)));
    connect(durationControls, SIGNAL(seek(int)),
            this, SLOT(timeSeek(int)));

    connect(this, SIGNAL(durationChanged(qint64)),
            durationControls, SLOT(songChanged(qint64)));
    connect(player, SIGNAL(metaDataChanged()),
            this, SLOT(metaDataChanged()));

    connect(menu, SIGNAL(play()),
            player, SLOT(play()));
    connect(menu, SIGNAL(pause()),
            player, SLOT(pause()));
    connect(menu, SIGNAL(gotoNextSong()),
            this, SLOT(nextSong()));
    connect(menu, SIGNAL(gotoPreviousSong()),
            this, SLOT(previousSong()));
    connect(menu, SIGNAL(updateLibrary()),
            library, SLOT(openDirectory()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            menu, SLOT(playPauseChangeText(QMediaPlayer::State)));

    connect(library, SIGNAL(libraryUpdated()),
            this, SLOT(updatePlaylist()));

    connect(ui->libraryView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customMenuRequested(QPoint)));
    connect(rightClickMenu, SIGNAL(playThisNow()),
            this, SLOT(playNow()));

    connect(this, SIGNAL(informationChanged(QString, QString)),
            information, SLOT(updateLabels(QString, QString)));
}

void PlayerWindow::setUpLayouts()
{
    QHBoxLayout *centralPlayerControlsLayout = new QHBoxLayout;
    centralPlayerControlsLayout->addStretch(1);
    centralPlayerControlsLayout->addSpacing(1);
    centralPlayerControlsLayout->addWidget(playerControls);
    centralPlayerControlsLayout->addStretch(1);
    centralPlayerControlsLayout->addSpacing(1);
    centralPlayerControlsLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *playerDurationControlsLayout = new QVBoxLayout;
    playerDurationControlsLayout->addLayout(centralPlayerControlsLayout);
    playerDurationControlsLayout->addWidget(durationControls);
    playerDurationControlsLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(information);
    controlLayout->addLayout(playerDurationControlsLayout);
    controlLayout->addWidget(volumeControls);
    controlLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *controlsWidget = new QWidget(this);
    controlsWidget->setLayout(controlLayout);
    controlsWidget->setStyleSheet("background: #ECF0F1;");

    QVBoxLayout *coverArtArea = new QVBoxLayout;
    coverArtArea->addStretch(1);
    coverArtArea->addSpacing(1);
    coverArtArea->addWidget(coverArtLabel);
    coverArtArea->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *uiLayout = new QHBoxLayout;
    uiLayout->addLayout(coverArtArea);
    uiLayout->addWidget(ui->libraryView, 1);
    uiLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *endLayout = new QVBoxLayout;
    endLayout->addLayout(uiLayout);
    endLayout->addWidget(controlsWidget);
    endLayout->setContentsMargins(0, 0, 0, 0);

    ui->libraryView->setStyleSheet("color: #ECF0F1; selection-background-color: #666666;");

    ui->centralWidget->setLayout(endLayout);
}
