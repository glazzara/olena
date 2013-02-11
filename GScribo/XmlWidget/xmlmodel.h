#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>

#include "xmlitem.h"

class XmlModel :
        public QAbstractItemModel
{
    public:
        explicit XmlModel(XmlItem *rootItem = 0, QObject *parent = 0);
        explicit XmlModel(XmlItem *rootItem, QAbstractItemModelPrivate& dd, QObject *parent = 0);
        ~XmlModel();

        void load(XmlItem *rootItem);
        void clear();

        inline int rowCount(const QModelIndex& parent) const;
        inline int columnCount(const QModelIndex& parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;

        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(int row, int column, const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& child) const;

    private:
        inline XmlItem *toXmlItem(const QModelIndex& index) const;

        XmlItem *rootItem_;
};

inline XmlItem *XmlModel::toXmlItem(const QModelIndex& parent) const
{ return !parent.isValid() ? rootItem_ : static_cast<XmlItem *>(parent.internalPointer()); }

inline int XmlModel::rowCount(const QModelIndex& parent) const
{ if(rootItem_) return toXmlItem(parent)->childs().count(); return 0; }

inline int XmlModel::columnCount(const QModelIndex&/* parent*/) const
{ return 1; }

#endif // XMLMODEL_H
