#ifndef ATTRIBUTESMODEL_H
#define ATTRIBUTESMODEL_H

#include <QAbstractItemModel>

#include "xmlattributes.h"

class AttributesModel :
        public QAbstractItemModel
{
        Q_OBJECT

    public:
        explicit AttributesModel(const XmlAttributes& attributes = XmlAttributes(), QObject *parent = 0);
        explicit AttributesModel(const XmlAttributes& attributes, QAbstractItemModelPrivate& dd, QObject *parent = 0);

        inline int rowCount(const QModelIndex& parent) const;
        inline int columnCount(const QModelIndex& parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QVariant data(const QModelIndex& index, int role) const;

        inline QModelIndex index(int row, int column, const QModelIndex &parent) const;
        inline QModelIndex parent(const QModelIndex &child) const;

    public slots:
        void reset();

    private:
        XmlAttributes attributes_;

    public slots:
        void load(const XmlAttributes& attributes);
};

inline int AttributesModel::rowCount(const QModelIndex&) const
{ return attributes_.names().count(); }

inline int AttributesModel::columnCount(const QModelIndex&) const
{ return 2; }

inline QModelIndex AttributesModel::index(int row, int column, const QModelIndex& parent) const
{ if(!hasIndex(row, column, parent)) return QModelIndex(); return createIndex(row, column, 0); }

inline QModelIndex AttributesModel::parent(const QModelIndex&) const
{ return QModelIndex(); }

#endif // ATTRIBUTESMODEL_H
