#ifndef DURATIONSLIDER_HPP
#define DURATIONSLIDER_HPP

#include <QSlider>

class SensibleSlider : public QSlider
{
Q_OBJECT

signals:
        void seek(int);

public:
        SensibleSlider(QWidget *parent = nullptr, Qt::Orientation = Qt::Horizontal);

        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif //DURATIONSLIDER_HPP
