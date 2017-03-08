#ifndef MUSICPLAYER_RIGHTCLICKMENU_HPP
#define MUSICPLAYER_RIGHTCLICKMENU_HPP

#include <QMenu>

class RightClickMenu : public QMenu
{
Q_OBJECT

signals:
    void playThisNext();
    void playThisNow();

public:
    RightClickMenu(QWidget *parent = nullptr);

public slots:
    void display(QPoint point);

private:
    QAction *playAction;
};

#endif //MUSICPLAYER_RIGHTCLICKMENU_HPP
