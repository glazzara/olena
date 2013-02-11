#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include <QList>

class ListModel:
        public QAbstractListModel
{
        Q_OBJECT

    public:
        explicit ListModel(QObject *parent = 0);
        int rowCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role = Qt::DecorationRole) const;
        void addPixmap(const QString& filename, const QPixmap& pixmap);

    private:
        QList<QPixmap> pixmaps;
        QList<QString> filenames;
};

#endif // LISTMODEL_H
