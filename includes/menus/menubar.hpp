#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include <QList>
#include <QMediaPlayer>

class QMenu;
class QAction;
class QWidget;
class PlayerWindow;

class MenuBar : public QObject
{
Q_OBJECT

signals:
    void play();
    void pause();
    void gotoNextSong();
    void gotoPreviousSong();
    void updateLibrary();

public:
    MenuBar(PlayerWindow *parent);
    ~MenuBar();

    QList<QMenu *> &getAllMenus();

private slots:
    void playOrPause();
    void playPauseChangeText(QMediaPlayer::State);
    void playPreviousSong();
    void playNextSong();
    void libraryScanDirectory();

private:
    void setUpMenus();
    void setUpActions();
    void connectActions();

    QList<QMenu *> menus;

    PlayerWindow *parent;

    QMenu *fileMenu;
    QMenu *controlsMenu;

    QAction *scanDir;

    QAction *nextSong;
    QAction *previousSong;
    QAction *playPause;
};


#endif //MENUBAR_HPP
