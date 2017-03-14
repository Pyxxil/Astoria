#ifndef MUSICPLAYER_HOVERDELEGATE_HPP
#define MUSICPLAYER_HOVERDELEGATE_HPP

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

#endif //MUSICPLAYER_HOVERDELEGATE_HPP
