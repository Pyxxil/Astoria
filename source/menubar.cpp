//
// Created by pyxxil on 1/03/17.
//

#include "includes/menubar.hpp"

#include <QMenu>

#include "includes/playerwindow.hpp"
#include "includes/playercontrols.hpp"

MenuBar::MenuBar(PlayerWindow *parent, PlayerControls *controls)
    : parent(parent)
      , controls(controls)
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
    editMenu = new QMenu("&Edit");

    menus.append(fileMenu);
    menus.append(editMenu);
}

void MenuBar::setUpActions()
{
    nextSong = new QAction("Next Song");
    nextSong->setShortcut(Qt::Key_F6);
    connect(nextSong, &QAction::triggered, this, &MenuBar::playNextSong);

    playPause = new QAction(parent->playerState() == QMediaPlayer::PlayingState ? "Pause" : "Play");
    playPause->setShortcut(Qt::Key_F5);
    connect(playPause, &QAction::triggered, this, &MenuBar::playOrPause);
    connect(controls, SIGNAL(pause()), this, SLOT(playPauseChangeText()));
    connect(controls, SIGNAL(play()), this, SLOT(playPauseChangeText()));
}

void MenuBar::connectActions()
{
    fileMenu->addAction(nextSong);
    fileMenu->addAction(playPause);
}

void MenuBar::playNextSong()
{
    emit parent->nextSong();
}

void MenuBar::playOrPause()
{
    if (parent->playerState() == QMediaPlayer::PlayingState) {
        emit controls->pause();
    } else {
        emit controls->play();
    }
}

void MenuBar::playPauseChangeText()
{
    if (parent->playerState() == QMediaPlayer::PlayingState) {
        playPause->setText("Pause");
    } else {
        playPause->setText("Play");
    }
}
