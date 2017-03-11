#ifndef DURATIONCONTROLS_HPP
#define DURATIONCONTROLS_HPP

#include <QWidget>

class SensibleSlider;
class QLabel;

class DurationControls: public QWidget
{
Q_OBJECT

signals:
    void seek(int);

public:
    DurationControls(QWidget *parent = 0, int minWidth = 16777215, int maxWidth = 16777215);

    SensibleSlider *durationSlider;
    QLabel *currentTime;
    QLabel *totalDuration;
    qint64 duration;

public slots:
    void positionChanged(qint64);
    void songChanged(qint64);
    void durationSliderValueChanged(int);
};

#endif //DURATIONCONTROLS_HPP
