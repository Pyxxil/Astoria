#include "includes/trackinformation.hpp"

#include <QVBoxLayout>

TrackInformation::TrackInformation(QWidget *parent, int minWidth, int maxWidth)
        : QWidget(parent)
{
        // TODO: Make this text scrollable (Marquee style).
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

/**
 * Update the displayed text.
 * @param newArtist The new artist
 * @param newSongTitle The new song title
 */
void TrackInformation::updateLabels(QString newArtist, QString newSongTitle)
{
        songLabel->setText(newSongTitle);
        artistLabel->setText(newArtist);
}