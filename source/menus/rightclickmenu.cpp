#include "includes/menus/rightclickmenu.hpp"

RightClickMenu::RightClickMenu(QWidget *parent)
    : QMenu(parent)
{
    playAction = new QAction("Play this");
    connect(playAction, &QAction::triggered, this, &RightClickMenu::playThisNow);
    addAction(playAction);
}

void RightClickMenu::display(QPoint point)
{
    popup(point);
}
