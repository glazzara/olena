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
        QStringList filenames() const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DecorationRole) const;
        void addPixmap(const QString& filename, const QPixmap& pixmap);

    private:
        QList<QPixmap> pixmaps;
        QStringList paths;

    public slots:
        void removePixmap(const QModelIndex &parent);
};

#endif // LISTMODEL_H
