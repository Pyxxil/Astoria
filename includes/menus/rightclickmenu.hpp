#ifndef RIGHTCLICKMENU_HPP
#define RIGHTCLICKMENU_HPP

#include <QMenu>

#include "includes/library/song.hpp"

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
    void display(QPoint point, const Song &songAtRow);
    void editMetadata();

private:
    Song selectedSong;

    QAction *playAction;
    QAction *editMetadataAction;
};

#endif //RIGHTCLICKMENU_HPP
