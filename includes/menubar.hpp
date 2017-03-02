//
// Created by pyxxil on 1/03/17.
//

#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include <QList>

class QMenu;
class QAction;
class QWidget;
class PlayerWindow;
class PlayerControls;

class MenuBar : public QObject
{
Q_OBJECT

public:
    MenuBar(PlayerWindow *parent, PlayerControls *controls);

    QList<QMenu *> &getAllMenus();

private slots:
    void playOrPause();
    void playPauseChangeText();
    void playPreviousSong();
    void playNextSong();

private:
    void setUpMenus();
    void setUpActions();
    void connectActions();

    QList<QMenu *> menus;

    PlayerWindow *parent;
    PlayerControls *controls;

    QMenu *fileMenu;
    QMenu *controlsMenu;

    QAction *nextSong;
    QAction *previousSong;
    QAction *playPause;
};


#endif //MUSICPLAYER_MENUBAR_HPP
