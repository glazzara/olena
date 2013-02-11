#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QLabel>
#include <QList>

class ListModel:
        public QAbstractListModel
{
        Q_OBJECT

    public:
        explicit ListModel(QStringList list, QObject *parent = 0);
        ~ListModel();
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DecorationRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DecorationRole) const;

    private:
        QStringList list;
};

#endif // LISTMODEL_H
