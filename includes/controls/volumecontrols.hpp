#ifndef VOLUMECONTROLS_HPP
#define VOLUMECONTROLS_HPP

#include <QWidget>
#include <QIcon>

class QAbstractButton;
class SensibleSlider;
class QBoxLayout;

class VolumeControls : public QWidget
{
Q_OBJECT

signals:
        void changeVolume(int);
        void mute(bool);

public:
        explicit VolumeControls(QWidget *parent = 0, int volume = 100, bool mute = false,
                                int minWidth = 16777215, int maxWidth = 16777215);

        int getVolume() const;
        bool isMuted() const;
        QBoxLayout *layout;

public slots:
        void setVolume(int volume);
        void setMute(bool mute);
        void volumeSliderValueChanged();

private:
        bool mutedStatus;
        QAbstractButton *muteButton;
        SensibleSlider *volumeSlider;

        QIcon muteIcon;
        QIcon lowVolumeIcon;
        QIcon mediumVolumeIcon;
        QIcon highVolumeIcon;

        void changeVolumeIcon();

private slots:
        void muteButtonClicked();
};

#endif // VOLUMECONTROLS_HPP
