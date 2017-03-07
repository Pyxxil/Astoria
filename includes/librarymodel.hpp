#ifndef MUSICPLAYER_LIBRARY_HPP
#define MUSICPLAYER_LIBRARY_HPP

#include <QAbstractTableModel>
#include <QMediaPlaylist>

class LibraryModel: public QAbstractTableModel
{
Q_OBJECT

signals:
    void libraryUpdated(int oldCount, int newCount);

public:
    LibraryModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    QMediaPlaylist *playlist;

    void scanDirectory(QString &directory);

public slots:
    void openDirectory();
    void updateLibrary();

private:
    int columns;
    int rows;
    QList<QString> supportedFormats;

    QMap<QString, QMap<QString, QString>> library;
};


#endif //MUSICPLAYER_LIBRARY_HPP
