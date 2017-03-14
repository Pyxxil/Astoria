#include "includes/controls/volumecontrols.hpp"

#include <QToolButton>
#include <QBoxLayout>
#include <QAudio>

#include "includes/controls/sensibleslider.hpp"

VolumeControls::VolumeControls(QWidget *parent, int volume, bool mute, int minWidth, int maxWidth)
    : QWidget(parent),
      mutedStatus(mute),
      muteIcon(QPixmap(":/icons/Mute.png")),
      lowVolumeIcon(QPixmap(":/icons/VolumeLow.png")),
      mediumVolumeIcon(QPixmap(":/icons/VolumeMedium.png")),
      highVolumeIcon(QPixmap(":/icons/VolumeHigh.png"))
{
    muteButton = new QToolButton(this);
    muteButton->setIconSize(QSize(25, 25));
    connect(muteButton, SIGNAL(clicked()),
            this, SLOT(muteButtonClicked()));

    volumeSlider = new SensibleSlider(this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setTracking(true);
    connect(volumeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(volumeSliderValueChanged()));

    // Move it slightly away from the edge of the window
    setContentsMargins(0, 0, 10, 0);
    setMinimumWidth(minWidth);
    setMaximumWidth(maxWidth);

    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(muteButton);
    layout->addWidget(volumeSlider);
    setLayout(layout);

    setVolume(volume);
}

int VolumeControls::getVolume() const
{
    qreal linearVolume = QAudio::convertVolume(
        volumeSlider->value() / qreal(100),
        QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale
    );

    return qRound(linearVolume * 100);
}

void VolumeControls::changeVolumeIcon()
{
    if (volumeSlider->value() > 70) {
        muteButton->setIcon(highVolumeIcon);
    }
    else if (volumeSlider->value() > 30) {
        muteButton->setIcon(mediumVolumeIcon);
    }
    else {
        muteButton->setIcon(lowVolumeIcon);
    }
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

    if (!isMuted()) {
        changeVolumeIcon();
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
        muteButton->setIcon(muteIcon);
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
