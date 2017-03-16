#ifndef HOVERDELEGATE_HPP
#define HOVERDELEGATE_HPP

#include <QStyledItemDelegate>

class HoverDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
        HoverDelegate();

        void paint(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // HOVERDELEGATE_HPP
