#include "includes/delegates/hoverdelegate.hpp"

#include <QPainter>

HoverDelegate::HoverDelegate()
{

}

void HoverDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(option.palette.highlight().color());
    painter->drawRect(option.rect);
    painter->setPen(option.palette.highlightedText().color());
    painter->drawText(option.rect, index.data().toString());
    painter->restore();
}
