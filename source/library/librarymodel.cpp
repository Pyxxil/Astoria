#include "includes/library/librarymodel.hpp"

#include <QFileDialog>

#include "includes/library/musicscanner.hpp"

LibraryModel::LibraryModel()
        : rows(0)
{
        playlist = new QMediaPlaylist(this);

        columnHeaders = {
                "Artist",
                "Album",
                "Title",
                "Genre",
                "Duration",
        };

        supportedFormats = {
                "*.mp3",
                "*.m4a",
                "*.mp4",
                "*.snd",
                "*.au",
                "*.aac",
                "*.adts",
                "*.aif",
                "*.aiff",
                "*.aifc",
                "*.caf",
                "*.sd2",
                "*.wav",
#if defined(Q_OS_LINUX)
        "*.wma",
        "*.flv",
        "*.ogg",
        "*.flac"
#endif
        };

        sort = AToZ;
}

LibraryModel::~LibraryModel()
{
        delete playlist;
}

bool LibraryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
        return QAbstractItemModel::setData(index, value, role);
}

int LibraryModel::columnCount(const QModelIndex &parent) const
{
        (void) parent;
        return columnHeaders.length();
}

int LibraryModel::rowCount(const QModelIndex &parent) const
{
        (void) parent;
        return rows;
}

QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
        if (role==Qt::DisplayRole) {
                if (orientation==Qt::Horizontal) {
                        // TODO: Change this slightly so that columns can be added/removed.
                        switch (section) {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:return getColumnHeader(section);
                        default:return QVariant();
                        }
                }
        }
        return QVariant();
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const
{
        return QAbstractTableModel::flags(index);
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const
{
        if (index.isValid()) {
                if (role==Qt::DisplayRole) {
                        if (library.length()==0) {
                                return QVariant();
                        }

                        const QMap<QString, QString> &metadata = songAt(index.row()).getMetadata();
                        const QString columnHeader = getColumnHeader(index.column());

                        // TODO: Change this slightly so that columns can be added/removed.
                        switch (index.column()) {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:return metadata[columnHeader];
                        default:return QVariant();
                        }
                }
        }

        return QVariant();
}

void LibraryModel::openDirectory()
{
        QString dir = QFileDialog::getExistingDirectory(nullptr, "Path to Music Folder", QDir::homePath());
        scanDirectory(dir);
}

void LibraryModel::scanDirectory(QString &directory)
{
        QDir musicDirectory(directory);
        QFileInfoList musicFiles = musicDirectory.entryInfoList(supportedFormats);
        MusicScanner *scanner = new MusicScanner(musicFiles);
        connect(scanner, SIGNAL(passNewItems(QList<Song>)), this, SLOT(updateLibrary(QList<Song>)));
        connect(scanner, SIGNAL(finished()), scanner, SLOT(deleteLater()));
        scanner->start();
}

void LibraryModel::updateLibrary(QList<Song> newItems)
{
        /*
        if (library.count() == rows) {
            // TODO: Read below
            // There's no point in updating the library if it's still the same number of items
            // Then again, what if some were removed at the same time ... ? Without it, however,
            // it segfaults, so for now it stays.
            return;
        }
        */
        if (newItems.length()==0) {
                return;
        }

        beginInsertRows(QModelIndex(), rowCount(), newItems.count() + rows - 1);
        for (auto &song : newItems) {
                bool notInLibrary = true;
                for (auto &songInLibrary : library) {
                        if (song.filePath==songInLibrary.filePath) {
                                notInLibrary = false;
                                break;
                        }
                }

                if (notInLibrary) {
                        library.append(song);
                        playlist->addMedia(QUrl::fromLocalFile(song.filePath));
                }
        }
        endInsertRows();

        rows += newItems.count();

        emit libraryUpdated();
}

const QUrl LibraryModel::get(int row) const
{
        return QUrl::fromLocalFile(library.at(row).filePath);
}

const QString &LibraryModel::getColumnHeader(int column) const
{
        return columnHeaders.at(column);
}

void LibraryModel::sortByColumn(int column)
{

        switch (sort) {
        case AToZ:
                std::sort(library.begin(), library.end(),
                          [column, this](const Song &a, const Song &b) {
                                  return a.getMetadata()[getColumnHeader(column)] <
                                          b.getMetadata()[getColumnHeader(column)];
                          });
                sort = ZToA;
                break;
        case ZToA:
                std::sort(library.begin(), library.end(),
                          [column, this](const Song &a, const Song &b) {
                                  return a.getMetadata()[getColumnHeader(column)] >
                                          b.getMetadata()[getColumnHeader(column)];
                          });
                sort = AToZ;
                break;
        }

        emit dataChanged(QModelIndex(), QModelIndex());
}

void LibraryModel::updateMetadata()
{
        Song song(library.at(mightBeUpdated.row()));
        library.removeAt(mightBeUpdated.row());
        library.insert(mightBeUpdated.row(), song);
        emit dataChanged(QModelIndex(), QModelIndex());
}

void LibraryModel::indexMightBeUpdated(const QModelIndex &index)
{
        mightBeUpdated = index;
}
