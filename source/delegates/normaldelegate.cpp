#include "includes/delegates/normaldelegate.hpp"

#include <QPainter>

NormalDelegate::NormalDelegate()
{

}

void NormalDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(option.palette.background().color());
    painter->drawRect(option.rect);
    painter->setPen(option.palette.foreground().color());
    painter->drawText(option.rect, index.data().toString());
    painter->restore();
}
