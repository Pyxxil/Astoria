#include "includes/controls/volumecontrols.hpp"

#include <QToolButton>
#include <QBoxLayout>
#include <QAudio>
#include <QMediaPlayer>

#include "includes/controls/sensibleslider.hpp"
#include "includes/astoria.hpp"

VolumeControls::VolumeControls(QWidget *parent, int minWidth, int maxWidth)
        : QWidget(parent),
          mutedStatus(Astoria::getAudioInstance()->isMuted()),
          muteIcon(QPixmap(":/assets/icons/Mute.png")),
          lowVolumeIcon(QPixmap(":/assets/icons/VolumeLow.png")),
          mediumVolumeIcon(QPixmap(":/assets/icons/VolumeMedium.png")),
          highVolumeIcon(QPixmap(":/assets/icons/VolumeHigh.png"))
{
        volumeIndicatorButton = new QToolButton(this);
        volumeIndicatorButton->setIconSize(QSize(25, 25));
        connect(volumeIndicatorButton, SIGNAL(clicked()),
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
        layout->addWidget(volumeIndicatorButton);
        layout->addWidget(volumeSlider);
        setLayout(layout);

        setVolume(Astoria::getAudioInstance()->volume());
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
        if (isMuted()) {
                volumeIndicatorButton->setIcon(muteIcon);
                return;
        }

        if (volumeSlider->value() > 70) {
                volumeIndicatorButton->setIcon(highVolumeIcon);
        } else if (volumeSlider->value() > 30) {
                volumeIndicatorButton->setIcon(mediumVolumeIcon);
        } else {
                volumeIndicatorButton->setIcon(lowVolumeIcon);
        }
}

void VolumeControls::setVolume(int volume)
{
        qreal logarithmicVolume = QAudio::convertVolume(
                volume / qreal(100), QAudio::LinearVolumeScale,
                QAudio::LogarithmicVolumeScale
        );

        volumeSlider->setValue(qRound(logarithmicVolume * 100));

        if (volumeSlider->value() == 0) {
                emit mute(true);
        } else if (isMuted()) {
                emit mute(false);
        }

        changeVolumeIcon();
}

void VolumeControls::volumeSliderValueChanged()
{
        emit changeVolume(getVolume());
}

void VolumeControls::setMute(bool mute)
{
        if (mute != mutedStatus) {
                mutedStatus = mute;
                changeVolumeIcon();
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
