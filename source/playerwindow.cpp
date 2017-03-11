#include "includes/playerwindow.hpp"
#include "ui_playerwindow.h"

#include <QMediaMetaData>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableView>
#include <QMimeDatabase>

#include <mpegfile.h>
#include <attachedpictureframe.h>
#include <id3v2tag.h>
#include <id3v2extendedheader.h>
#include <mp4tag.h>
#include <mp4coverart.h>
#include <mp4file.h>

#include "includes/controls/durationcontrols.hpp"
#include "includes/controls/playercontrols.hpp"
#include "includes/controls/volumecontrols.hpp"
#include "includes/library/librarymodel.hpp"
#include "includes/menus/rightclickmenu.hpp"
#include "includes/trackinformation.hpp"
#include "includes/menus/menubar.hpp"


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
 *  - Set up a namespace that everything can connect to
 *      - Audio stuff
 *      - UI stuff
 */

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);

    QFile styleSheet(":/StyleSheet.qss");
    styleSheet.open(QFile::ReadOnly | QFile::Text);
    QTextStream styles(&styleSheet);
    qApp->setStyleSheet(styles.readAll());

    //setWindowTitle(Project);
    //setStyleSheet("background-color: #333333; font-size: 13pt;");

    player = new QMediaPlayer(this);

    playerControls = new PlayerControls(this, player->state());
    volumeControls = new VolumeControls(this, player->volume(),
                                        player->isMuted(), 150, 150);
    volumeControls->setContentsMargins(0, 0, 5, 0);
    durationControls = new DurationControls(this, 200);

    library = new LibraryModel();
    libraryView = new QTableView;

    player->setPlaylist(library->playlist);

    menu = new MenuBar(this);
    for (const auto &_menu : menu->getAllMenus()) {
        ui->menuBar->addMenu(_menu);
    }

    information = new TrackInformation(this, 200, 200);

    rightClickMenu = new RightClickMenu(this);

    coverArtLabel = new QLabel(this);
    coverArtLabel->setScaledContents(true);
    coverArtLabel->setBackgroundRole(QPalette::Base);
    coverArtLabel->setMinimumSize(200, 200);
    coverArtLabel->setMaximumSize(200, 200);
    coverArtLabel->setContentsMargins(0, 0, 0, 0);

    setupConnections();
    setupUI();
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

    loadCoverArt(song);
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
    player->playlist()->insertMedia(0, library->get(libraryView->currentIndex().row()));
    player->playlist()->setCurrentIndex(0);
    emit player->play();
}

void PlayerWindow::customMenuRequested(QPoint pos)
{
    if (libraryView->indexAt(pos).isValid()) {
        emit rightClickMenu->display(libraryView->viewport()->mapToGlobal(pos));
    }
}

void PlayerWindow::updatePlaylist()
{
    // TODO: Figure out what to do here
}

void PlayerWindow::setupConnections()
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

    connect(libraryView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customMenuRequested(QPoint)));
    connect(rightClickMenu, SIGNAL(playThisNow()),
            this, SLOT(playNow()));

    connect(this, SIGNAL(informationChanged(QString, QString)),
            information, SLOT(updateLabels(QString, QString)));

    connect(libraryView->horizontalHeader(), SIGNAL(sectionClicked(int)),
            library, SLOT(sortByColumn(int)));
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
    //controlsWidget->setStyleSheet("background-color: #ECF0F1; color: 333333;");

    QVBoxLayout *coverArtArea = new QVBoxLayout;
    coverArtArea->addStretch(1);
    coverArtArea->addSpacing(1);
    coverArtArea->addWidget(coverArtLabel);
    coverArtLabel->setContentsMargins(0, 0, 0, 0);
    coverArtLabel->setMaximumWidth(information->maximumWidth());
    coverArtArea->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *uiLayout = new QHBoxLayout;
    uiLayout->addLayout(coverArtArea);
    uiLayout->addWidget(libraryView, 1);
    uiLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *endLayout = new QVBoxLayout;
    endLayout->addLayout(uiLayout);
    endLayout->addWidget(controlsWidget);
    endLayout->setContentsMargins(0, 0, 0, 0);

    //libraryView->setStyleSheet("QTableView { color: #ECF0F1; selection-background-color: #666666; }\n"
    //                               "QHeaderView::section { background-color: #333333; color: #ECF0F1; border: none; }");
    libraryView->setModel(library);
    libraryView->horizontalHeader()->setFrameShape(QFrame::NoFrame);
    libraryView->setSortingEnabled(true);
    libraryView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    libraryView->horizontalHeader()->setHighlightSections(false);
    libraryView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    libraryView->horizontalHeader()->setFrameRect(QRect());
    libraryView->verticalHeader()->setVisible(false);
    libraryView->setContextMenuPolicy(Qt::CustomContextMenu);
    libraryView->setSelectionBehavior(QAbstractItemView::SelectRows);
    libraryView->setShowGrid(false);
    libraryView->setFrameShape(QFrame::NoFrame);
    libraryView->horizontalHeader()->setSectionsClickable(true);

    ui->centralWidget->setLayout(endLayout);
}

void PlayerWindow::loadCoverArt(TagLib::FileRef &song)
{
    QMimeDatabase db;
    QMimeType codec = db.mimeTypeForFile(song.file()->name());

    if (codec.name() == "audio/mp4") {
        TagLib::MP4::File mp4(song.file()->name());
        TagLib::MP4::Tag* tag = mp4.tag();
        TagLib::MP4::ItemListMap itemsListMap = tag->itemListMap();
        TagLib::MP4::Item coverItem = itemsListMap["covr"];
        TagLib::MP4::CoverArtList coverArtList = coverItem.toCoverArtList();

        if (!coverArtList.isEmpty()) {
            TagLib::MP4::CoverArt coverArt = coverArtList.front();
            image.loadFromData((const uchar *)
                                   coverArt.data().data(),coverArt.data().size());
        } else {
            image.load(":/CoverArtUnavailable.png");
        }
    } else if (codec.name() == "audio/mpeg"){
        TagLib::MPEG::File file(song.file()->name());
        TagLib::ID3v2::Tag *m_tag = file.ID3v2Tag();
        TagLib::ID3v2::FrameList frameList = m_tag->frameList("APIC");

        if (!frameList.isEmpty()) {
            TagLib::ID3v2::AttachedPictureFrame
                *coverImg = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());

            image.loadFromData((const uchar *) coverImg->picture().data(), coverImg->picture().size());
        } else {
            image.load(":/CoverArtUnavailable.png");
        }
    } else {
        image.load(":/CoverArtUnavailable.png");
    }

    coverArtLabel->setPixmap(QPixmap::fromImage(image));
}
