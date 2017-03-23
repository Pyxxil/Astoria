#include "includes/playerwindow.hpp"
#include "ui_playerwindow.h"

#include <QMediaMetaData>
#include <QMimeDatabase>
#include <QHBoxLayout>
#include <QTableView>
#include <QLabel>

#include "includes/controls/durationcontrols.hpp"
#include "includes/controls/playercontrols.hpp"
#include "includes/controls/volumecontrols.hpp"
#include "includes/library/librarymodel.hpp"
#include "includes/menus/rightclickmenu.hpp"
#include "includes/library/libraryview.hpp"
#include "includes/trackinformation.hpp"
#include "includes/menus/menubar.hpp"
#include "includes/coverartlabel.hpp"
#include "includes/astoria.hpp"

/*
 * TODO: Possible features
 * To add features list
 *	- Allow the stylesheet to be read from a file (.qss file) (This is sort of done already)
 *	    - Might have some default themes (Dark, Light, etc.)
 *	- User adjustable previous song time limit (that is, how long into a song does pressing
 *	  the previous button restart the song)
 *	- Allow the user to remove/add headers and change the order of them
 *	    - These can be picked from whatever metadata can be grabbed
 *	- Saveability
 *	    - Save state
 *	    - Save library
 *	    - Save playlist's
 *	- Edit file metadata [*]
 *
 * TODO: Fixes
 *  - Fix up UI stuff (Most is done, just got to fix the gap between the cover art and the library)
 *  - Add other tags to columns in the library view (Partly done)
 *  - Fix the Playlist problems (playing next, previous crashes, change playlist set-up)
 *  - Set up a namespace that everything can connect to
 *      - Audio stuff
 *        - Global media player
 *      - UI stuff
 *        - Width & height percentages
 *  - Catch the window closing to do the following:
 *      - Saving
 *      - De-initialising items in the global namespace
 */

PlayerWindow::PlayerWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::PlayerWindow)
{
        ui->setupUi(this);

        // Project is defined in CMakeLists.txt
        setWindowTitle(Project);

        QFile styleSheet(":/assets/StyleSheet.qss");
        styleSheet.open(QFile::ReadOnly | QFile::Text);
        QTextStream styles(&styleSheet);
        qApp->setStyleSheet(styles.readAll());

        Astoria::init();

        playerControls = new PlayerControls(this);
        volumeControls = new VolumeControls(this, 150, 150);
        volumeControls->setContentsMargins(0, 0, 5, 0);
        durationControls = new DurationControls(this, 200);

        library = new LibraryModel;
        libraryView = new LibraryView(this, library);

        menu = new MenuBar(this);
        for (const auto &_menu : menu->getAllMenus()) {
                ui->menuBar->addMenu(_menu);
        }

        information = new TrackInformation(this, 200, 200);

        rightClickMenu = new RightClickMenu(this);

        coverArtLabel = new CoverArtLabel(this);

        setupConnections();
        setupUI();
}

PlayerWindow::~PlayerWindow()
{
        delete ui;
}

void PlayerWindow::nextSong()
{
        if (!Astoria::getAudioInstance()->playlist()->isEmpty()) {
                Astoria::getAudioInstance()->playlist()->next();
        }
}

/*
 * Go to the previous song, unless the time played into the current song is
 * less than 10 seconds.
 */
void PlayerWindow::previousSong()
{
        // TODO: Make the time to go to the previous song adjustable
        // TODO: What to do if the user has pressed go to previous and there aren't any songs
        // TODO: before it? Do we set position to 0, and pause the media? Or just reset the song?
        QMediaPlayer *player = Astoria::getAudioInstance();
        if (player->position() < 10000 && player->playlist()->currentIndex() > 0) {
                player->playlist()->previous();
        } else {
                player->setPosition(0);
        }
}

void PlayerWindow::timeSeek(int time)
{
        Astoria::getAudioInstance()->setPosition(time);
}

/*
 * In general, whenever the metadata updates, we want to update a few things:
 *      - The duration displayed by the duration controls
 *              - Either a new song is being played, and the duration is different than the previous
 *              - Or metadata has been edited to provide a new duration.
 *      - The window title, as this displays the current playing song
 *      - The information displayed below the cover art, and to the left of the controls.
 *      - We also want to update the cover art.
 */
void PlayerWindow::metaDataChanged()
{
        emit durationChanged(Astoria::getAudioInstance()->duration());
        TagLib::FileRef song(
                QStringToTString(
                        Astoria::getCurrentSong()
                                .toString()
                                .remove(0, 7))  // Remove the file:// prefix
                        .toCString());

        setWindowTitle(QString("%1 - %2")
                               .arg(TStringToQString(song.tag()->artist()))
                               .arg(TStringToQString(song.tag()->title())));

        emit songChanged(Astoria::getCurrentTag());
}

/*
 * Either begin playing the song, or continue playing the current song (the latter should be less
 * likely of an occurrence).
 */
void PlayerWindow::play()
{
        if (!Astoria::getPlaylistInstance()->isEmpty()) {
                emit Astoria::getAudioInstance()->play();
        }
}

/**
 * Play a song right now.
 */
void PlayerWindow::playNow()
{
        // TODO: Fix this.
        Astoria::getPlaylistInstance()->insertMedia(0, library->get(libraryView->currentIndex().row()));
        Astoria::getPlaylistInstance()->setCurrentIndex(0);
        emit Astoria::getAudioInstance()->play();
}

/**
 * When the user clicks on the library, we want to show them a menu that they can use.
 * @param pos Where the user clicked.
 */
void PlayerWindow::customMenuRequested(QPoint pos)
{
        if (libraryView->indexAt(pos).isValid()) {
                library->indexMightBeUpdated(libraryView->indexAt(pos));
                emit rightClickMenu->display(libraryView->viewport()->mapToGlobal(pos),
                                             library->songAt(libraryView->indexAt(pos).row()));
        }
}

void PlayerWindow::updatePlaylist()
{
        // TODO: Figure out what to do here
}

void PlayerWindow::setupConnections()
{
        // TODO: Think about moving some of these into their respective classes
        QMediaPlayer *player = Astoria::getAudioInstance();

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
        connect(player, SIGNAL(metaDataChanged()),
                this, SLOT(metaDataChanged()));
        connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
                menu, SLOT(playPauseChangeText(QMediaPlayer::State)));

        connect(durationControls, SIGNAL(seek(int)),
                this, SLOT(timeSeek(int)));

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

        connect(library, SIGNAL(libraryUpdated()),
                this, SLOT(updatePlaylist()));

        connect(this, SIGNAL(songChanged(TagLib::FileRef)),
                information, SLOT(updateLabels(TagLib::FileRef)));
        connect(this, SIGNAL(durationChanged(qint64)),
                durationControls, SLOT(songChanged(qint64)));

        connect(libraryView, SIGNAL(customContextMenuRequested(QPoint)),
                this, SLOT(customMenuRequested(QPoint)));
        connect(libraryView->horizontalHeader(), SIGNAL(sectionClicked(int)),
                library, SLOT(sortByColumn(int)));
        connect(libraryView, SIGNAL(doubleClicked(const QModelIndex &)),
                this, SLOT(playNow()));

        connect(rightClickMenu, SIGNAL(playThisNow()),
                this, SLOT(playNow()));
        connect(rightClickMenu, SIGNAL(updateLibrary()),
                library, SLOT(updateMetadata()));

        connect(this, SIGNAL(songChanged(TagLib::FileRef)),
                coverArtLabel, SLOT(artChanged(TagLib::FileRef)));
}

void PlayerWindow::setupUI()
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
        controlsWidget->setMaximumHeight(120);
        controlsWidget->setContentsMargins(0, 0, 0, 0);
        controlsWidget->setStyleSheet("border: none;");

        QVBoxLayout *coverArtArea = new QVBoxLayout;
        coverArtArea->setContentsMargins(0, 0, 0, 0);
        coverArtArea->addStretch(1);
        coverArtArea->addSpacing(1);
        coverArtArea->addWidget(coverArtLabel);

        QHBoxLayout *uiLayout = new QHBoxLayout;
        uiLayout->addLayout(coverArtArea);
        uiLayout->addWidget(libraryView, 1);
        uiLayout->setContentsMargins(0, 0, 0, 0);

        QVBoxLayout *endLayout = new QVBoxLayout;
        endLayout->addLayout(uiLayout);
        endLayout->addWidget(controlsWidget);
        endLayout->setContentsMargins(0, 0, 0, 0);

        ui->centralWidget->setLayout(endLayout);
}

void PlayerWindow::closeEvent(QCloseEvent *event)
{
        Astoria::deInit();
        QMainWindow::closeEvent(event);
}
