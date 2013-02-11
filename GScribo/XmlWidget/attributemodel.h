#ifndef ATTRIBUTEMODEL_H
#define ATTRIBUTEMODEL_H

#include <QAbstractItemModel>

#include "xmlattributes.h"

class AttributeModel :
        public QAbstractItemModel
{
    public:
        explicit AttributeModel(const XmlAttributes& attributes = XmlAttributes(), QObject *parent = 0);
        explicit AttributeModel(const XmlAttributes& attributes, QAbstractItemModelPrivate& dd, QObject *parent = 0);

        void load(const XmlAttributes& attributes);

        inline int rowCount(const QModelIndex& parent) const;
        inline int columnCount(const QModelIndex& parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QVariant data(const QModelIndex& index, int role) const;

        inline QModelIndex index(int row, int column, const QModelIndex &parent) const;
        inline QModelIndex parent(const QModelIndex &child) const;

        void reset();

    private:
        XmlAttributes attributes_;
};

inline int AttributeModel::rowCount(const QModelIndex&) const
{ return attributes_.names().count(); }

inline int AttributeModel::columnCount(const QModelIndex&) const
{ return 2; }

inline QModelIndex AttributeModel::index(int row, int column, const QModelIndex& parent) const
{ if(!hasIndex(row, column, parent)) return QModelIndex(); return createIndex(row, column, 0); }

inline QModelIndex AttributeModel::parent(const QModelIndex&) const
{ return QModelIndex(); }

#endif // ATTRIBUTEMODEL_H
