#ifndef RIGHTCLICKMENU_HPP
#define RIGHTCLICKMENU_HPP

#include <QMenu>

class RightClickMenu: public QMenu
{
Q_OBJECT

signals:
    void playThisNext();
    void playThisNow();

public:
    RightClickMenu(QWidget *parent = nullptr);
    ~RightClickMenu();

public slots:
    void display(QPoint point);

private:
    QAction *playAction;
};

#endif //RIGHTCLICKMENU_HPP
