#include "includes/delegates/hoverdelegate.hpp"

#include <QPainter>
#include <QApplication>

HoverDelegate::HoverDelegate()
{

}

void HoverDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        if (option.state & QStyle::State_Selected) {
                QStyledItemDelegate::paint(painter, option, index);
        } else {
                QStyleOptionViewItem opt = option;
                initStyleOption(&opt, index);
                QStyle *style = QApplication::style();
                painter->save();
                painter->setPen(Qt::NoPen);
                painter->setBrush(QBrush("#34373a"));
                painter->drawRect(option.rect);
                painter->setPen(opt.palette.highlightedText().color());
                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
                painter->restore();
        }
}
