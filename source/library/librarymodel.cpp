#include "includes/library/librarymodel.hpp"

#include <QFileDialog>
#include <QMediaPlayer>

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
    return QAbstractItemModel::headerData(section, orientation, role);
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
            return QString();
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
    QMediaPlayer player;
    QDir musicDirectory(directory);
    QFileInfoList musicFiles = musicDirectory.entryInfoList(supportedFormats);
    MusicScanner *scanner = new MusicScanner(library, musicFiles);
    connect(scanner, SIGNAL(finished()), this, SLOT(updateLibrary()));
    connect(scanner, SIGNAL(finished()), scanner, SLOT(deleteLater()));
    scanner->start();
}

void LibraryModel::updateLibrary()
{
    if (library.count() == rows) {
        // TODO: Read below
        // There's no point in updating the library if it's still the same number of items
        // Then again, what if some were removed at the same time ... ? Without it, however,
        // it segfaults, so for now it stays.
        return;
    }
    beginInsertRows(QModelIndex(), rows, library.count() - rows);
    insertRows(rows, library.count() - rows);
    endInsertRows();
    emit dataChanged(QModelIndex(), QModelIndex());
    rows = library.count();
    for (const auto &song : library) {
        playlist->addMedia(QUrl::fromLocalFile(song.filePath));
    }
    emit libraryUpdated();
}

const QUrl LibraryModel::get(int row) const
{
    return QUrl::fromLocalFile(library.at(row).filePath);
}
