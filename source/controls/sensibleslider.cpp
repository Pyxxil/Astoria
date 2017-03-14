#include "includes/controls/sensibleslider.hpp"

#include <QStyleOptionSlider>
#include <QMouseEvent>
#include <QDebug>

/**
 * A sensible version of QT's QSlider class that actually sets the value
 * when it's clicked (it should also take into account the handle).
 * @param parent The parent of this widget.
 * @param orientation The orientation of the widget.
 */
SensibleSlider::SensibleSlider(QWidget *parent, Qt::Orientation orientation)
        : QSlider(orientation, parent)
{

}

/**
 *
 * @param event
 */
void SensibleSlider::mousePressEvent(QMouseEvent *event)
{
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

        if (event->button()==Qt::LeftButton && !handle.contains(event->pos())) {
                int newVal;
                if (orientation()==Qt::Vertical) {
                        newVal = minimum() + ((maximum() - minimum())*(height() - event->y()))/height();
                } else {
                        newVal = minimum() + ((maximum() - minimum())*event->x())/width();
                }

                if (invertedAppearance()) {
                        setValue(maximum() - newVal);
                } else {
                        setValue(newVal);
                }

                event->accept();
                emit seek(newVal*1000);
        }
        QSlider::mousePressEvent(event);
}
