#include "includes/playerwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerWindow w;
    w.show();

    return a.exec();
}
