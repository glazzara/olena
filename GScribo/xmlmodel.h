#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>

#include "xmlitem.h"

class XmlModel :
        public QAbstractItemModel
{
    public:
        explicit XmlModel(XmlItem *rootItem, QObject *parent = 0);
        explicit XmlModel(XmlItem *rootItem, QAbstractItemModelPrivate& dd, QObject *parent = 0);
        ~XmlModel();

        inline int rowCount(const QModelIndex& parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(int row, int column, const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& child) const;

    private:
        inline XmlItem *toXmlItem(const QModelIndex& index) const;

        XmlItem *rootItem_;
};

inline XmlItem *XmlModel::toXmlItem(const QModelIndex& parent) const
{ return parent.isValid() ? rootItem_ : static_cast<XmlItem *>(parent.internalPointer()); }

inline int XmlModel::rowCount(const QModelIndex& parent) const
{ toXmlItem(parent)->childs().count(); }

#endif // XMLMODEL_H
