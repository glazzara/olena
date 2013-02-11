#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>

class XmlModel :
        public QAbstractItemModel
{
    public:
        explicit XmlModel(QObject *parent);
        ~XmlModel();

        inline int rowCount(const QModelIndex &parent) const;
        inline int columnCount(const QModelIndex &parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QVariant data(const QModelIndex &index, int role) const;

        QModelIndex index(int row, int column, const QModelIndex &parent) const;
        QModelIndex parent(const QModelIndex &child) const;

    private:

};

#endif // XMLMODEL_H
