#include "includes/metadataeditordialog.hpp"
#include "ui_metadataeditordialog.h"

#include <QPushButton>
#include <QDebug>

MetadataEditorDialog::MetadataEditorDialog(QWidget *parent)
        : QDialog(parent),
          ui(new Ui::MetadataEditorDialog),
          edits(0)
{
        ui->setupUi(this);
}

MetadataEditorDialog::~MetadataEditorDialog()
{
        delete ui;
}

void MetadataEditorDialog::setupMetadata(Song &song)
{
        metadata = song.getMetadata();
        file = song.filePath;
        ui->songTitleTextbox->setText(metadata["Title"]);
        ui->artistTextbox->setText(metadata["Artist"]);
        ui->albumTextbox->setText(metadata["Album"]);
        ui->trackTextbox->setText(metadata["Track"]);
        ui->genreTextbox->setText(metadata["Genre"]);
        ui->yearTextbox->setText(metadata["Year"]);

        ui->trackErrorLabel->setStyleSheet("color: red");
        ui->yearErrorLabel->setStyleSheet("color: red");

        saveButton = ui->buttonBox->button(QDialogButtonBox::Save);
        saveButton->setEnabled(false);

        connect(ui->trackTextbox, SIGNAL(textChanged(const QString &)),
                this, SLOT(trackTextChanged(const QString &)));
        connect(ui->albumTextbox, SIGNAL(textChanged(const QString &)),
                this, SLOT(albumTextChanged(const QString &)));
        connect(ui->artistTextbox, SIGNAL(textChanged(const QString &)),
                this, SLOT(artistTextChanged(const QString &)));
        connect(ui->genreTextbox, SIGNAL(textChanged(const QString &)),
                this, SLOT(genreTextChanged(const QString &)));
        connect(ui->songTitleTextbox, SIGNAL(textChanged(const QString &)),
                this, SLOT(songTitleTextChanged(const QString &)));
        connect(ui->yearTextbox, SIGNAL(textChanged(const QString &)),
                this, SLOT(yearTextChanged(const QString &)));

        connect(this, SIGNAL(accepted()),
                this, SLOT(save()));
}

void MetadataEditorDialog::albumTextChanged(const QString &)
{
        if (ui->albumTextbox->text() == metadata["Album"]) {
                edits ^= AlbumText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                return;
        }

        edits |= AlbumText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::trackTextChanged(const QString &newText)
{
        bool converted = false;

        if (newText == metadata["Track"]) {
                edits ^= TrackText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                ui->trackErrorLabel->setText("");
                return;
        }

        newText.toUInt(&converted);
        if (!converted) {
                ui->trackErrorLabel->setText("Invalid track number");
                edits ^= TrackText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                return;
        }

        ui->trackErrorLabel->setText("");

        edits |= TrackText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::artistTextChanged(const QString &newText)
{
        if (newText == metadata["Artist"]) {
                edits ^= ArtistText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                return;
        }

        edits |= ArtistText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::genreTextChanged(const QString &newText)
{
        if (newText == metadata["Genre"]) {
                edits ^= GenreText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                return;
        }

        edits |= GenreText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::songTitleTextChanged(const QString &newText)
{
        if (newText == metadata["Title"]) {
                edits ^= TitleText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                return;
        }

        edits |= TitleText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::yearTextChanged(const QString &newText)
{
        bool converted = false;

        if (newText == metadata["Year"]) {
                edits ^= YearText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                return;
        }

        newText.toUInt(&converted);
        if (!converted) {
                ui->yearErrorLabel->setText("Invalid year");
                edits ^= YearText;
                if (edits == 0) {
                        saveButton->setEnabled(false);
                }
                return;
        }

        ui->yearErrorLabel->setText("");
        edits |= YearText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

/*
 * Save any changed data to disk, and update the library.
 */

void MetadataEditorDialog::save()
{
        if (edits == 0) {
                // This should always evaluate to false, but it's being kept
                // in as a fail safe.
                return;
        }

        TagLib::FileRef songFile(file.toStdString().c_str());

        if (edits & ArtistText) {
                songFile.tag()->setArtist(QStringToTString(ui->artistTextbox->text()));
        }

        if (edits & AlbumText) {
                songFile.tag()->setAlbum(QStringToTString(ui->albumTextbox->text()));
        }

        if (edits & TrackText) {
                songFile.tag()->setTrack(ui->trackTextbox->text().toUInt());
        }

        if (edits & GenreText) {
                songFile.tag()->setGenre(QStringToTString(ui->genreTextbox->text()));
        }

        if (edits & TitleText) {
                songFile.tag()->setTitle(QStringToTString(ui->songTitleTextbox->text()));
        }

        if (edits & YearText) {
                songFile.tag()->setYear(ui->yearTextbox->text().toUInt());
        }

        songFile.save();
        emit libraryNeedsUpdating();
}
