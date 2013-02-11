#include "xmlmodel.h"

XmlModel::XmlModel(XmlItem *rootItem, QObject *parent) :
        QAbstractItemModel(parent)
{
    this->rootItem = rootItem;
}

XmlModel::XmlModel(XmlItem *rootItem, QAbstractItemModelPrivate &dd, QObject *parent) :
        QAbstractItemModel(dd, parent)
{
    this->rootItem = rootItem;
}

XmlModel::~XmlModel()
{
    delete rootItem;
}

XmlItem *XmlModel::xmlItem(const QModelIndex &parent) const
{
    // Check if we are in the root item.
    if(!parent.isValid())
        return rootItem;
    else
        // Get xml item from the model index.
        return static_cast<XmlItem *>(parent.internalPointer());
}

QVariant XmlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return tr("Tree");
            case 1:
                return tr("Name");
            case 2:
                return tr("Value");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QVariant XmlModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::UserRole)
        return static_cast<XmlItem *>(index.internalPointer())->text();

    /*if(role == Qt::UserRole)
        static_cast<XmlItem *>(index.internalPointer())->text();

    if(role == Qt::UserRole+1)
        static_cast<XmlItem *>(index.internalPointer())->attributes();

    if(role == Qt::UserRole+2)
        VariantPointer::toQVariant(static_cast<XmlItem *>(index.internalPointer())->graphicalItem());*/

    return QVariant();
}

QModelIndex XmlModel::index(int row, int column, const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    XmlItem *parentItem, *childItem;

    // Check if we are in the root item.
    if(!parent.isValid())
        parentItem = rootItem;
    else
        // Get xml item from the model index.
        parentItem = static_cast<XmlItem *>(parent.internalPointer());

    childItem = parentItem->child(row);

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

    if(!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->rowCount(), 0, parentItem);
}
