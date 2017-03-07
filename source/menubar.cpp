#include "includes/menubar.hpp"

#include <QMenu>

#include "includes/playerwindow.hpp"

MenuBar::MenuBar()
{
    setUpMenus();
    setUpActions();
    connectActions();
}

QList<QMenu *> &MenuBar::getAllMenus()
{
    return menus;
}

void MenuBar::setUpMenus()
{
    fileMenu = new QMenu("&File");
    controlsMenu = new QMenu("Controls");

    menus.append(fileMenu);
    menus.append(controlsMenu);
}

void MenuBar::setUpActions()
{
    scanDir = new QAction("Scan Directory");
    scanDir->setShortcut(Qt::Key_Open);
    connect(scanDir, &QAction::triggered,
            this, &MenuBar::libraryScanDirectory);

    previousSong = new QAction("Previous Song");
    previousSong->setShortcut(Qt::Key_F4);
    connect(previousSong, &QAction::triggered,
            this, &MenuBar::playPreviousSong);

    nextSong = new QAction("Next Song");
    nextSong->setShortcut(Qt::Key_F6);
    connect(nextSong, &QAction::triggered,
            this, &MenuBar::playNextSong);

    // Should probably change this...
    playPause = new QAction("Play");
    playPause->setShortcut(Qt::Key_F5);
    connect(playPause, &QAction::triggered,
            this, &MenuBar::playOrPause);
}

void MenuBar::connectActions()
{
    fileMenu->addAction(scanDir);

    controlsMenu->addAction(previousSong);
    controlsMenu->addAction(playPause);
    controlsMenu->addAction(nextSong);
}

void MenuBar::playOrPause()
{
    if (parent->playerState() == QMediaPlayer::PlayingState) {
        emit pause();
    } else {
        emit play();
    }
    playPauseChangeText();
}

void MenuBar::playPauseChangeText()
{
    if (parent->playerState() == QMediaPlayer::PlayingState) {
        playPause->setText("Pause");
    } else {
        playPause->setText("Play");
    }
}

void MenuBar::playNextSong()
{
    emit gotoNextSong();
}

void MenuBar::playPreviousSong()
{
    emit gotoPreviousSong();
}

void MenuBar::libraryScanDirectory()
{
    emit updateLibrary();
}
