#include "xmlmodel.h"

XmlModel::XmlModel(XmlItem *rootItem, QObject *parent) :
        QAbstractItemModel(parent)
{
    rootItem_ = rootItem;
}

XmlModel::XmlModel(XmlItem *rootItem, QAbstractItemModelPrivate& dd, QObject *parent) :
        QAbstractItemModel(dd, parent)
{
    rootItem_ = rootItem;
}

XmlModel::~XmlModel()
{
    if(rootItem_)
        delete rootItem_;
}

QVariant XmlModel::headerData(int, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Tree");

    return QVariant();
}

QVariant XmlModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role)
    {
        case Qt::UserRole:
            return static_cast<XmlItem *>(index.internalPointer())->text();

        case Qt::UserRole+1:
            return static_cast<XmlItem *>(index.internalPointer())->isSelected();

        case Qt::UserRole+2:
            return QVariant::fromValue(static_cast<XmlItem *>(index.internalPointer())->attributes());

        case Qt::UserRole+3:
            return VariantPointer<PolygonItem>::toQVariant(static_cast<XmlItem *>(index.internalPointer())->graphicalItem());

        default:
            return QVariant();
    }
}

QModelIndex XmlModel::index(int row, int column, const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    XmlItem *childItem = toXmlItem(parent)->child(row);

    // Create the model index from the child item.
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex XmlModel::parent(const QModelIndex& child) const
{
    if(!child.isValid())
        return QModelIndex();

    XmlItem *parentItem = static_cast<XmlItem *>(child.internalPointer())->parent();

    if(!parentItem || parentItem == rootItem_)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

void XmlModel::load(XmlItem *rootItem)
{
    beginResetModel();

    if(rootItem_)
        delete rootItem_;

    rootItem_ = rootItem;

    endResetModel();
}
