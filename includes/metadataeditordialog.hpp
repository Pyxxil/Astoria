#ifndef METADATAEDITORDIALOG_HPP
#define METADATAEDITORDIALOG_HPP

#include <QDialog>

#include "includes/library/song.hpp"

namespace Ui
{
        class MetadataEditorDialog;
}

class MetadataEditorDialog : public QDialog
{
Q_OBJECT

signals:
        void libraryNeedsUpdating();

public:
        explicit MetadataEditorDialog(QWidget *parent = 0);
        ~MetadataEditorDialog();

        void setupMetadata(Song &song);

public slots:
        void albumTextChanged(const QString &);
        void trackTextChanged(const QString &);
        void artistTextChanged(const QString &);
        void genreTextChanged(const QString &);
        void songTitleTextChanged(const QString &);
        void yearTextChanged(const QString &);
        void save();

private:
        Ui::MetadataEditorDialog *ui;

        enum TextEdited
        {
                ArtistText = 1 << 0,
                AlbumText = 1 << 1,
                TrackText = 1 << 2,
                GenreText = 1 << 3,
                TitleText = 1 << 4,
                YearText = 1 << 5,
        };

        quint16 edits;

        QMap<QString, QString> metadata;
        QString file;

        QPushButton *saveButton;
};

#endif // METADATAEDITORDIALOG_HPP
