#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>

// Taglib, at least on OSX, throws a couple of deprecated declaration warnings
// which are annoying to see, and interfere with -Werror. This might not be a
// good thing to do, but it solves this problem for now.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include "taglib.h"
#include "fileref.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

class TrackInformation;
class DurationControls;
class RightClickMenu;
class VolumeControls;
class PlayerControls;
class CoverArtLabel;
class LibraryModel;
class QTableView;
class MenuBar;

namespace Ui
{
        class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
Q_OBJECT

signals:
        void durationChanged(qint64);
        void songChanged(TagLib::FileRef);

public:
        explicit PlayerWindow(QWidget *parent = 0);
        ~PlayerWindow();

public slots:
        void nextSong();
        void previousSong();
        void timeSeek(int);
        void metaDataChanged();
        void playNow();
        void customMenuRequested(QPoint pos);
        void updatePlaylist();
        void play();

private:
        Ui::PlayerWindow *ui;

        LibraryModel *library;
        DurationControls *durationControls;
        VolumeControls *volumeControls;
        PlayerControls *playerControls;
        TrackInformation *information;
        MenuBar *menu;
        RightClickMenu *rightClickMenu;

        CoverArtLabel *coverArtLabel;
        QTableView *libraryView;

        QImage image;

        void setupConnections();
        void setupUI();

protected:
        void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
};

#endif // PLAYERWINDOW_H
