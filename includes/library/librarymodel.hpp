#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <QAbstractTableModel>
#include <QMediaPlaylist>

#include "includes/library/song.hpp"

/**
 * TODO: Change most of this.
 *
 *  - There should be a playlist class that holds information on the current playlist.
 *  - The scanner should provide a list containing new elements (While checking the current
 *    library for any duplicates).
 *    - This should help in adding the new elements, and only the new elements, to the
 *      playlist.
 *  - The library should only be used as a model, not a controller of logic.
 *
 * FEATURES:
 *  - MenuBar
 *    - Choice of highlighting currently playing media (or some icon).
 *    - Choice of view follows playing media.
 *  - Right Click Menu
 *  - Both
 *    - Remove Item from library
 *
 */

class LibraryModel : public QAbstractTableModel
{
Q_OBJECT

signals:
        void libraryUpdated();

public:
        explicit LibraryModel();
        ~LibraryModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

        Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

        void scanDirectory(QString &directory);
        void indexMightBeUpdated(const QModelIndex &index);

        const QUrl get(int row) const;

        const Song &songAt(int row) const
        { return library[row]; }

public slots:
        void openDirectory();
        void updateLibrary(QList<Song>);
        void sortByColumn(int column);
        void updateMetadata();

private:
        int rows;
        QList<QString> supportedFormats;

        QList<Song> library;

        enum SortType
        {
                AToZ,
                ZToA,
        };

        SortType sort;
        const QString &getColumnHeader(int column) const;
        QList<QString> columnHeaders;

        QModelIndex mightBeUpdated;
};

#endif //LIBRARY_HPP
