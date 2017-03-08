#ifndef MUSICPLAYER_DURATIONCONTROLS_HPP
#define MUSICPLAYER_DURATIONCONTROLS_HPP

#include <QWidget>

class QLabel;
class DurationSlider;

class DurationControls : public QWidget
{
Q_OBJECT

signals:
    void seek(int);

public:
    DurationControls(QWidget *parent = 0);

    DurationSlider *durationSlider;
    QLabel *currentTime;
    QLabel *totalDuration;
    qint64 duration;

public slots:
    void positionChanged(qint64);
    void songChanged(qint64);
    void durationSliderValueChanged(int);
};

#endif //MUSICPLAYER_DURATIONCONTROLS_HPP
