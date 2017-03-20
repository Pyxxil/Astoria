#include "includes/metadataeditordialog.hpp"
#include "ui_metadataeditordialog.h"

#include <QPushButton>
#include <QDebug>

#define EDITED(flag) edits |= (flag);         \
        if (!saveButton->isEnabled()) {       \
                saveButton->setEnabled(true); \
        }

#define UNEDIT(flag) edits ^= (flag);          \
        if (edits == 0) {                      \
                saveButton->setEnabled(false); \
        }

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
                UNEDIT(AlbumText);
                return;
        }

        EDITED(AlbumText);
}

void MetadataEditorDialog::trackTextChanged(const QString &newText)
{
        bool converted = false;

        if (newText == metadata["Track"]) {
                UNEDIT(TrackText);
                ui->trackErrorLabel->setText("");
                return;
        }

        newText.toUInt(&converted);
        if (!converted) {
                ui->trackErrorLabel->setText("Invalid track number");
                UNEDIT(TrackText);
                return;
        }

        ui->trackErrorLabel->setText("");

       EDITED(TrackText);
}

void MetadataEditorDialog::artistTextChanged(const QString &newText)
{
        if (newText == metadata["Artist"]) {
                UNEDIT(ArtistText);
                return;
        }

        EDITED(ArtistText);
}

void MetadataEditorDialog::genreTextChanged(const QString &newText)
{
        if (newText == metadata["Genre"]) {
                UNEDIT(GenreText);
                return;
        }

        EDITED(GenreText);
}

void MetadataEditorDialog::songTitleTextChanged(const QString &newText)
{
        if (newText == metadata["Title"]) {
                UNEDIT(TitleText);
                return;
        }

        EDITED(TitleText);
}

void MetadataEditorDialog::yearTextChanged(const QString &newText)
{
        bool converted = false;

        if (newText == metadata["Year"]) {
                UNEDIT(YearText);
                return;
        }

        newText.toUInt(&converted);
        if (!converted) {
                ui->yearErrorLabel->setText("Invalid year");
                UNEDIT(YearText);
                return;
        }

        ui->yearErrorLabel->setText("");
        EDITED(YearText);
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
