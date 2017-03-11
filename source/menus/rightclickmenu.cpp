#include "includes/menus/rightclickmenu.hpp"

RightClickMenu::RightClickMenu(QWidget *parent)
    : QMenu(parent)
{
    playAction = new QAction("Play this");
    connect(playAction, &QAction::triggered, this, &RightClickMenu::playThisNow);
    addAction(playAction);

    //setStyleSheet("QMenu::item:hover { background-color: #666666; }\nQMenu::Item { color: #ECF0F1; }");
}

RightClickMenu::~RightClickMenu()
{
    delete playAction;
}

void RightClickMenu::display(QPoint point)
{
    popup(point);
}
