#include "includes/controls/durationcontrols.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QMediaContent>

#include "includes/playerwindow.hpp"
#include "includes/controls/durationslider.hpp"

DurationControls::DurationControls(QWidget *parent)
    : QWidget(parent), duration(0)
{
    durationSlider = new DurationSlider(this);
    connect(durationSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(durationSliderValueChanged(int)));
    connect(durationSlider, SIGNAL(seek(int)), this, SIGNAL(seek(int)));
    durationSlider->setValue(0);
    setStyleSheet("QSlider::Handle {image: none;}");

    currentTime = new QLabel(this);
    currentTime->setText("0:00");

    totalDuration = new QLabel(this);
    totalDuration->setText("0:00");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(currentTime);
    layout->addWidget(durationSlider);
    layout->addWidget(totalDuration);

    setLayout(layout);
}

void DurationControls::positionChanged(qint64 position)
{
    if (duration > 0) {
        durationSlider->setValue(static_cast<int>(position / 1000));
        currentTime->setText(QString("%1:%2").arg(position / 1000 / 60).arg((position / 1000) % 60, 2, 10, QChar('0')));
    }
}

void DurationControls::songChanged(qint64 newDuration)
{
    if (newDuration != duration) {
        duration = newDuration;
        totalDuration->setText(QString("%1:%2").arg(newDuration / 1000 / 60).arg((newDuration / 1000) % 60, 2, 10, QChar('0')));
        durationSlider->setMaximum(static_cast<int>(newDuration / 1000));
    }
}

void DurationControls::durationSliderValueChanged(int newValue)
{
    emit seek(newValue * 1000);
}
