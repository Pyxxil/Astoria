#include "includes/menus/rightclickmenu.hpp"

#include "includes/metadataeditordialog.hpp"

RightClickMenu::RightClickMenu(QWidget *parent)
    : QMenu(parent),
      selectedSong(QFileInfo("/dev/null"))
{
    playAction = new QAction("Play this");
    connect(playAction, &QAction::triggered,
            this, &RightClickMenu::playThisNow);
    addAction(playAction);

    editMetadataAction = new QAction("Edit metadata");
    connect(editMetadataAction, &QAction::triggered,
            this, &RightClickMenu::editMetadata);
    addAction(editMetadataAction);
}

RightClickMenu::~RightClickMenu()
{
    delete playAction;
}

void RightClickMenu::display(QPoint point, const Song &songAtRow)
{
    selectedSong = songAtRow;
    popup(point);
}

void RightClickMenu::editMetadata()
{
    MetadataEditorDialog dialog(this);
    connect(&dialog, SIGNAL(libraryNeedsUpdating()),
            this, SIGNAL(updateLibrary()));
    dialog.setupMetadata(selectedSong);
    dialog.exec();
}
