#include "includes/trackinformation.hpp"

#include <QVBoxLayout>

TrackInformation::TrackInformation(QWidget *parent, int minWidth, int maxWidth)
    : QWidget(parent)
{
    songLabel = new QLabel(this);
    artistLabel = new QLabel(this);

    // Move it a little closer to the controls to it's right.
    setContentsMargins(0, 0, -10, 0);

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