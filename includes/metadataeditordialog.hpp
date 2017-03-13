#ifndef METADATAEDITORDIALOG_HPP
#define METADATAEDITORDIALOG_HPP

#include <QDialog>

#include "includes/library/song.hpp"

namespace Ui {
class MetadataEditorDialog;
}

class MetadataEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetadataEditorDialog(QWidget *parent = 0);
    ~MetadataEditorDialog();

    void setupMetadata(Song &song);

private:
    Ui::MetadataEditorDialog *ui;
};

#endif // METADATAEDITORDIALOG_HPP
