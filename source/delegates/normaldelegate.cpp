#include "includes/delegates/normaldelegate.hpp"

#include <QPainter>

NormalDelegate::NormalDelegate()
{

}

void NormalDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
