#include "includes/library/librarymodel.hpp"

#include <QFileDialog>

#include "includes/library/musicscanner.hpp"

LibraryModel::LibraryModel()
    : columns(5), rows(0)
{
    playlist = new QMediaPlaylist(this);

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
    return columns;
}

int LibraryModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return rows;
}

QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return QString("Artist");
            case 1:
                return QString("Album");
            case 2:
                return QString("Title");
            case 3:
                return QString("Genre");
            case 4:
                return QString("Duration");
            default:
                return QVariant();
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
    if (index.isValid() && role == Qt::DisplayRole) {
        if (library.length() == 0) {
            return QVariant();
        }
        const QMap<QString, QString> &metadata = library.at(index.row()).metaData();
        switch (index.column()) {
        case 0:
            return metadata["Artist"];
        case 1:
            return metadata["Album"];
        case 2:
            return metadata["Title"];
        case 3:
            return metadata["Genre"];
        case 4:
            return metadata["Duration"];
        default:
            return QVariant();
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
    if (newItems.count() == 0) {
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), newItems.count() + rows - 1);
    for (const auto &song : newItems) {
        library.append(song);
        playlist->addMedia(QUrl::fromLocalFile(song.filePath));
    }
    endInsertRows();

    rows += newItems.count();

    emit libraryUpdated();
}

const QUrl LibraryModel::get(int row) const
{
    return QUrl::fromLocalFile(library.at(row).filePath);
}

void LibraryModel::sortByColumn(int column)
{
    switch (sort) {
    case AToZ:
        std::sort(library.begin(), library.end(),
                  [column](Song &a, Song &b){
                      return a.metaData().keys().at(column) > b.metaData().keys().at(column);
                  });
        sort = ZToA;
        break;
    case ZToA:
        std::sort(library.begin(), library.end(),
                  [column](const auto &a, const auto &b){
                      return a.metaData().keys().at(column) < b.metaData().keys().at(column);
                  });
        sort = AToZ;
        break;
    }

    emit dataChanged(QModelIndex(), QModelIndex());
}
