#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>
#include <QIcon>

class QAbstractButton;

class PlayerControls: public QWidget
{
Q_OBJECT

signals:
    void play();
    void pause();
    void next();
    void previous();

public:
    explicit PlayerControls(QWidget *parent = 0, QMediaPlayer::State state = QMediaPlayer::StoppedState);

public slots:
    void setState(QMediaPlayer::State state);

private:
    QMediaPlayer::State playerState;
    QAbstractButton *playPauseButton;
    QAbstractButton *nextButton;
    QAbstractButton *previousButton;
    QAbstractButton *repeatButton;
    QAbstractButton *shuffleButton;

    QIcon playPauseIcon;
    QIcon nextIcon;
    QIcon previousIcon;
    QIcon shuffleIcon;
    QIcon repeatIcon;

private slots:
    void playPauseButtonClicked();
    void nextButtonClicked();
    void previousButtonClicked();
};

#endif // PLAYERCONTROLS_H
