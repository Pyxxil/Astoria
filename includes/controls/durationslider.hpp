#ifndef MUSICPLAYER_DURATIONSLIDER_HPP
#define MUSICPLAYER_DURATIONSLIDER_HPP

#include <QSlider>

class DurationSlider : public QSlider
{
Q_OBJECT

signals:
    void seek(int);

public:
    DurationSlider(QWidget *parent = nullptr, Qt::Orientation = Qt::Horizontal);

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};


#endif //MUSICPLAYER_DURATIONSLIDER_HPP
