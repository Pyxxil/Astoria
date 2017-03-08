#include "includes/controls/durationslider.hpp"

#include <QStyleOptionSlider>
#include <QMouseEvent>
#include <QDebug>

DurationSlider::DurationSlider(QWidget *parent, Qt::Orientation orientation)
    : QSlider(orientation, parent)
{

}

void DurationSlider::mousePressEvent(QMouseEvent *event)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    if (event->button() == Qt::LeftButton && !sr.contains(event->pos()))
    {
        int newVal;
        if (orientation() == Qt::Vertical) {
            newVal = minimum() + ((maximum() - minimum()) * (height() - event->y())) / height();
        }
        else {
            newVal = minimum() + ((maximum() - minimum()) * event->x()) / width();
        }

        if (invertedAppearance()) {
            setValue(maximum() - newVal);
        }
        else {
            setValue(newVal);
        }

        event->accept();
        emit seek(newVal * 1000);
    }
    QSlider::mousePressEvent(event);
}
