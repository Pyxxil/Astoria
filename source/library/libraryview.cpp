#include "includes/library/libraryview.hpp"

#include <QHeaderView>

LibraryView::LibraryView(QWidget *parent, QAbstractItemModel *library)
    : QTableView(parent)
{
    setMouseTracking(true);

    connect(this, SIGNAL(entered(const QModelIndex &)),
            this, SLOT(entry(const QModelIndex &)));

    if (library != nullptr) {
        setModel(library);
    }

    horizontalHeader()->setFrameShape(QFrame::NoFrame);
    setSortingEnabled(true);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setFrameRect(QRect());
    verticalHeader()->setVisible(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(false);
    setFrameShape(QFrame::NoFrame);
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
