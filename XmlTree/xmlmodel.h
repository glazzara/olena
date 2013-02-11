#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>

#include "xmlitem.h"

class XmlModel :
        public QAbstractItemModel
{
    public:
        explicit XmlModel(XmlItem *rootItem, QObject *parent = 0);
        explicit XmlModel(XmlItem *rootItem, QAbstractItemModelPrivate& dd, QObject *parent);
        ~XmlModel();

        inline int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(int row, int column, const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& child) const;

    private:
        XmlItem *xmlItem(const QModelIndex& parent) const;

        XmlItem *rootItem;
};

inline int XmlModel::rowCount(const QModelIndex& parent) const
{ XmlItem *parentItem = xmlItem(parent); return parentItem->childs().count(); }

inline int XmlModel::columnCount(const QModelIndex&/* parent*/) const
{ return 1; }

#endif // XMLMODEL_H
