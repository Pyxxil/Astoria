#include "includes/librarymodel.hpp"

#include <QFileDialog>
#include <QMediaPlayer>

#include "includes/musicscanner.hpp"

LibraryModel::LibraryModel()
    : columns(5), rows(0)
{
    playlist = new QMediaPlaylist(this);
    playlist->addMedia(QList<QMediaContent>(
        // TODO: Remove the below when everything is loaded later.
#ifdef Q_OS_LINUX
    {
        QUrl::fromLocalFile("/home/pyxxil/Downloads/01 - Welcome to the Black Parade.mp3"),
        QUrl::fromLocalFile("/home/pyxxil/Downloads/01. The Writer (Starsmith Edit).mp3"),
        QUrl::fromLocalFile("/home/pyxxil/Downloads/02 - Heaven Help Us.mp3"),
        QUrl::fromLocalFile("/home/pyxxil/Downloads/02. The Writer (Album Version).mp3"),
        QUrl::fromLocalFile("/home/pyxxil/Downloads/03. The Writer (Instrumental).mp3"),
    }
#endif
    ));

#if defined(Q_OS_LINUX)

#elif defined(Q_OS_MACOS)

#elif defined(Q_OS_WIN)

#else
#error "Unsupported platform"
#endif
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
    (void) index;
    (void) role;
    if (index.isValid()) {
        switch (index.column()) {
        case 0:
            if (library.keys().count()) {
                return library.values().at(index.row())["Artist"];
            }
            return QVariant();
        case 1:
            if (library.values().count()) {
                return library.values().at(index.row())["Album"];
            }
            return QVariant();
        case 2:
            return QString();
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
    QFileInfoList musicFiles = musicDirectory.entryInfoList({"*.mp3"});
    MusicScanner *scanner = new MusicScanner(&library, musicFiles);
    scanner->start();
    connect(scanner, SIGNAL(finished()), this, SLOT(updateLibrary()));
}

void LibraryModel::updateLibrary()
{
    emit libraryUpdated(rows, library.count());
    beginInsertRows(QModelIndex(), rows, library.count() - rows);
    insertRows(rows, library.count() - rows);
    endInsertRows();
    rows = library.count();
}
