#ifndef MUSICPLAYER_NORMALDELEGATE_HPP
#define MUSICPLAYER_NORMALDELEGATE_HPP

#include <QStyledItemDelegate>

class NormalDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
        NormalDelegate();
        void paint(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif //MUSICPLAYER_NORMALDELEGATE_HPP
