#include "includes/metadataeditordialog.hpp"
#include "ui_metadataeditordialog.h"

#include <QPushButton>
#include <QDebug>

// TODO: Maybe change it to check for saving while in the slot's,
// TODO: this way there aren't any checks after saving, and the save
// TODO: button can stay/be disabled if the string in the text box
// TODO: is the same as the string already in the metadata.

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
        ui->durationTextbox->setText(metadata["Duration"]);
        ui->genreTextbox->setText(metadata["Genre"]);

        ui->durationErrorLabel->setStyleSheet("color: red");
        ui->trackErrorLabel->setStyleSheet("color: red");

        saveButton = ui->buttonBox->button(QDialogButtonBox::Save);
        saveButton->setEnabled(false);

        connect(ui->trackTextbox, SIGNAL(textChanged(
                                                 const QString &)),
                this, SLOT(trackTextChanged(
                                   const QString &)));
        connect(ui->albumTextbox, SIGNAL(textChanged(
                                                 const QString &)),
                this, SLOT(albumTextChanged(
                                   const QString &)));
        connect(ui->artistTextbox, SIGNAL(textChanged(
                                                  const QString &)),
                this, SLOT(artistTextChanged(
                                   const QString &)));
        connect(ui->durationTextbox, SIGNAL(textChanged(
                                                    const QString &)),
                this, SLOT(durationTextChanged(
                                   const QString &)));
        connect(ui->genreTextbox, SIGNAL(textChanged(
                                                 const QString &)),
                this, SLOT(genreTextChanged(
                                   const QString &)));
        connect(ui->songTitleTextbox, SIGNAL(textChanged(
                                                     const QString &)),
                this, SLOT(songTitleTextChanged(
                                   const QString &)));

        connect(this, SIGNAL(accepted()),
                this, SLOT(save()));
}

void MetadataEditorDialog::albumTextChanged(const QString &)
{
        if (ui->albumTextbox->text()==metadata["Album"]) {
                edits ^= AlbumText;
                saveButton->setEnabled(edits > 0);
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
        qDebug() << newText;

        if (newText==0) {
                ui->trackErrorLabel->setText("Invalid track number");
                edits ^= TrackText;
                return;
        }

        if (newText==metadata["Track"]) {
                edits ^= TrackText;
                saveButton->setEnabled(edits > 0);
                ui->trackErrorLabel->setText("");
                return;
        }

        newText.toInt(&converted);
        if (!converted) {
                ui->trackErrorLabel->setText("Invalid track number");
                edits ^= TrackText;
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
        if (newText==metadata["Artist"]) {
                edits ^= ArtistText;
                saveButton->setEnabled(edits > 0);
                return;
        }

        edits |= ArtistText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::durationTextChanged(const QString &newText)
{
        bool minutesConverted = false;
        bool secondsConverted = false;

        if (newText==metadata["Duration"]) {
                edits ^= DurationText;
                saveButton->setEnabled(edits > 0);
                ui->durationErrorLabel->setText("");
                return;
        }

        QStringList time = newText.split(':');
        if (time.length()!=2) {
                ui->durationErrorLabel->setText("Invalid duration");
                edits ^= DurationText;
                return;
        }

        (void) time[0].toInt(&minutesConverted);
        (void) time[1].toInt(&secondsConverted);

        if (!(minutesConverted && secondsConverted)) {
                ui->durationErrorLabel->setText("Invalid duration");
                edits ^= DurationText;
                return;
        }

        ui->durationErrorLabel->setText("");

        edits |= DurationText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::genreTextChanged(const QString &newText)
{
        if (newText==metadata["Genre"]) {
                edits ^= GenreText;
                saveButton->setEnabled(edits > 0);
                return;
        }

        edits |= GenreText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::songTitleTextChanged(const QString &newText)
{
        if (newText==metadata["Title"]) {
                edits ^= TitleText;
                saveButton->setEnabled(edits > 0);
                return;
        }

        edits |= TitleText;
        if (!saveButton->isEnabled()) {
                saveButton->setEnabled(true);
        }
}

void MetadataEditorDialog::save()
{
        // TODO: this probably isn't really scalable.
        TagLib::FileRef songFile(file.toStdString().c_str());

        if (edits & ArtistText) {
                if (ui->artistTextbox->text()!=metadata["Artist"]) {
                        songFile.tag()->setArtist(ui->artistTextbox->text().toStdString().c_str());
                }
        }

        if (edits & AlbumText) {
                if (ui->albumTextbox->text()!=metadata["Album"]) {
                        songFile.tag()->setAlbum(ui->albumTextbox->text().toStdString().c_str());
                }
        }

        if (edits & TrackText) {
                if (ui->trackTextbox->text()!=metadata["Track"]) {

                }
        }

        if (edits & DurationText) {
                if (ui->durationTextbox->text()!=metadata["Duration"]) {

                }
        }

        if (edits & GenreText) {
                if (ui->genreTextbox->text()!=metadata["Genre"]) {

                }
        }

        if (edits & TitleText) {
                if (ui->songTitleTextbox->text()!=metadata["Title"]) {

                }
        }

        if (edits) {
                songFile.save();
                emit libraryNeedsUpdating();
        }
}
