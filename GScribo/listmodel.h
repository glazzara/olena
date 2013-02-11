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
        explicit ListModel(QStringList stringList, QObject *parent = 0);
        ~ListModel();
        int rowCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role = Qt::DecorationRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DecorationRole) const;
        /*Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DecorationRole);*/

    private:
        QStringList stringList;
};

#endif // LISTMODEL_H
