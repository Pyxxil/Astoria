#include "includes/metadataeditordialog.hpp"
#include "ui_metadataeditordialog.h"

MetadataEditorDialog::MetadataEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MetadataEditorDialog)
{
    ui->setupUi(this);
}

MetadataEditorDialog::~MetadataEditorDialog()
{
    delete ui;
}

void MetadataEditorDialog::setupMetadata(Song &song)
{
    QMap<QString, QString> metadata = song.metaData();
    ui->songTitleTextbox->setText(metadata["Title"]);
    ui->artistTextbox->setText(metadata["Artist"]);
    ui->albumTextbox->setText(metadata["Album"]);
    ui->trackTextbox->setText(metadata["Track"]);
    ui->durationTextbox->setText(metadata["Duration"]);
    ui->genreTextbox->setText(metadata["Genre"]);
}
