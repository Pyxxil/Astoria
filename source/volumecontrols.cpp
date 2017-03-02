#include "includes/volumecontrols.hpp"

#include <QToolButton>
#include <QSlider>
#include <QBoxLayout>
#include <QStyle>
#include <QAudio>

VolumeControls::VolumeControls(QWidget *parent)
    : QWidget(parent), mutedStatus(false)
{
    muteButton = new QToolButton(this);
    muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    muteButton->setStyleSheet("border: none;");
    connect(muteButton, SIGNAL(clicked()),
            this, SLOT(muteButtonClicked()));

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setTracking(true);
    connect(volumeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(volumeSliderValueChanged()));

    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(muteButton);
    layout->addWidget(volumeSlider);
    setLayout(layout);
}

int VolumeControls::getVolume() const
{
    qreal linearVolume = QAudio::convertVolume(
        volumeSlider->value() / qreal(100), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale
    );

    return qRound(linearVolume * 100);
}

void VolumeControls::setVolume(int volume)
{
    qreal logarithmicVolume = QAudio::convertVolume(
        volume / qreal(100), QAudio::LinearVolumeScale, QAudio::LogarithmicVolumeScale
    );

    volumeSlider->setValue(qRound(logarithmicVolume * 100));

    if (volumeSlider->value() == 0) {
        emit mute(true);
    }
    else if (isMuted()) {
        emit mute(false);
    }
}

void VolumeControls::volumeSliderValueChanged()
{
    emit changeVolume(getVolume());
}

void VolumeControls::setMute(bool mute)
{
    if (mute != mutedStatus) {
        mutedStatus = mute;
        muteButton->setIcon(
            style()->standardIcon(
                mute ? QStyle::SP_MediaVolumeMuted :
                    QStyle::SP_MediaVolume));
    }
}

void VolumeControls::muteButtonClicked()
{
    emit mute(!isMuted());
}

bool VolumeControls::isMuted() const
{
    return mutedStatus;
}
