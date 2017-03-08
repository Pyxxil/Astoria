#ifndef VOLUMECONTROLS_HPP
#define VOLUMECONTROLS_HPP

#include <QWidget>

class QAbstractButton;
class QAbstractSlider;
class QBoxLayout;

class VolumeControls: public QWidget
{
Q_OBJECT

signals:
    void changeVolume(int);
    void mute(bool);

public:
    explicit VolumeControls(QWidget *parent = 0);

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
    QAbstractSlider *volumeSlider;

private slots:
    void muteButtonClicked();
};


#endif // VOLUMECONTROLS_HPP
