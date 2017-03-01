#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>

class QAbstractButton;
class QBoxLayout;

class PlayerControls: public QWidget
{
Q_OBJECT

signals:
    void play();
    void pause();
    void next();
    void previous();

public:
    explicit PlayerControls(QWidget *parent = 0);
    QBoxLayout *layout;

public slots:
    void setState(QMediaPlayer::State state);

private:
    QMediaPlayer::State playerState;
    QAbstractButton *playPauseButton;
    QAbstractButton *nextButton;
    QAbstractButton *previousButton;

private slots:
    void playPauseButtonClicked();
    void nextButtonClicked();
    void previousButtonClicked();
};

#endif // PLAYERCONTROLS_H
