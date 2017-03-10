#include "includes/trackinformation.hpp"

#include <QVBoxLayout>

TrackInformation::TrackInformation(QWidget *parent)
    : QWidget(parent)
{
    songLabel = new QLabel(this);
    artistLabel = new QLabel(this);

    setLayout(new QVBoxLayout());
    layout()->addWidget(songLabel);
    layout()->addWidget(artistLabel);

    setMaximumWidth(200);
    setMinimumWidth(150);
}

void TrackInformation::updateLabels(QString artist, QString song)
{
    songLabel->setText(song);
    artistLabel->setText(artist);
}