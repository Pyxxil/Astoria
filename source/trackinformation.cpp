#include "includes/trackinformation.hpp"

#include <QVBoxLayout>

TrackInformation::TrackInformation(QWidget *parent, int minWidth, int maxWidth)
    : QWidget(parent)
{
    songLabel = new QLabel(this);
    artistLabel = new QLabel(this);

    setLayout(new QVBoxLayout());
    layout()->addWidget(songLabel);
    layout()->addWidget(artistLabel);

    setMinimumWidth(minWidth);
    setMaximumWidth(maxWidth);
}

void TrackInformation::updateLabels(QString artist, QString song)
{
    // TODO: Make this text scrollable (Marquee style).
    songLabel->setText(song);
    artistLabel->setText(artist);
}