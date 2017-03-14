#ifndef MUSICPLAYER_TRACKINFORMATION_HPP
#define MUSICPLAYER_TRACKINFORMATION_HPP

#include <QWidget>
#include <QLabel>

class TrackInformation: public QWidget
{
Q_OBJECT

public:
    TrackInformation(QWidget *parent = nullptr, int minWidth = 16777215, int maxWidth = 16777215);

public slots:
    void updateLabels(QString, QString);

private:
    QLabel *songLabel;
    QLabel *artistLabel;
};


#endif //MUSICPLAYER_TRACKINFORMATION_HPP
