#include "includes/library/libraryview.hpp"

LibraryView::LibraryView(QWidget *parent)
    : QTableView(parent)
{
    setMouseTracking(true);

    connect(this, SIGNAL(entered(const QModelIndex &)),
            this, SLOT(entry(const QModelIndex &)));
}

void LibraryView::entry(const QModelIndex &index)
{
    if (currentIndex != -1) {
        setDelegate(currentIndex, false);
    }

    setDelegate(index.row(), true);
    currentIndex = index.row();
}

void LibraryView::setDelegate(int row, bool doHover)
{
    if (doHover) {
        setItemDelegateForRow(row, &hoverDelegate);
    } else {
        setItemDelegateForRow(row, &normalDelegate);
    }
}
