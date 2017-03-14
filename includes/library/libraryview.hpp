#ifndef MUSICPLAYER_LIBRARYVIEW_HPP
#define MUSICPLAYER_LIBRARYVIEW_HPP

#include <QTableView>

#include "includes/delegates/hoverdelegate.hpp"
#include "includes/delegates/normaldelegate.hpp"

class LibraryView : public QTableView
{
Q_OBJECT

public:
        LibraryView(QWidget *parent = nullptr, QAbstractItemModel *library = nullptr);
        void setDelegate(int row, bool doHover);

public slots:
        void entry(const QModelIndex &);

private:
        int currentIndex = -1;
        HoverDelegate hoverDelegate;
        NormalDelegate normalDelegate;
};

#endif //MUSICPLAYER_LIBRARYVIEW_HPP
