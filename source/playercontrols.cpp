#include "includes/playercontrols.hpp"

#include <QToolButton>
#include <QBoxLayout>
#include <QStyle>

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent), playerState(QMediaPlayer::StoppedState)
{
    playPauseButton = new QToolButton(this);
    playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playPauseButton, SIGNAL(clicked()),
            this, SLOT(playPauseButtonClicked()));

    nextButton = new QToolButton(this);
    nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(nextButton, SIGNAL(clicked()),
            this, SLOT(nextButtonClicked()));

    previousButton = new QToolButton(this);
    previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(previousButton, SIGNAL(clicked()),
            this, SLOT(previousButtonClicked()));

    layout = new QHBoxLayout(this);
    layout->addWidget(previousButton);
    layout->addWidget(playPauseButton);
    layout->addWidget(nextButton);

    setLayout(layout);
}

void PlayerControls::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
            case QMediaPlayer::PlayingState:
                playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
                break;
            case QMediaPlayer::PausedState:
            case QMediaPlayer::StoppedState:
                playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
                break;
        }
    }
}

void PlayerControls::playPauseButtonClicked()
{
    switch (playerState) {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            emit play();
            break;
        case QMediaPlayer::PlayingState:
            emit pause();
            break;
    }
}

void PlayerControls::nextButtonClicked()
{
    emit next();
}

void PlayerControls::previousButtonClicked()
{
    emit previous();
}
