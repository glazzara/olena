#include "xmlmodel.h"

XmlModel::XmlModel(XmlItem *rootItem, QObject *parent) :
        QAbstractItemModel(parent)
{
    rowCount_ = 0;
    rootItem_ = 0;
}

XmlModel::XmlModel(XmlItem *rootItem, QAbstractItemModelPrivate& dd, QObject *parent) :
        QAbstractItemModel(dd, parent)
{
    rowCount_ = 0;
    rootItem_ = 0;
}

XmlModel::~XmlModel()
{
    if(rootItem_)
        delete rootItem_;
}

QVariant XmlModel::headerData(int/* section*/, Qt::Orientation orientation, int role) const
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
        return list_.at(index.row())->text();
            //return static_cast<XmlItem *>(index.internalPointer())->text();

        case Qt::UserRole+1:
            //return static_cast<XmlItem *>(index.internalPointer())->attributes().names();

        case Qt::UserRole+2:
            //return static_cast<XmlItem *>(index.internalPointer())->attributes().values();

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

void XmlModel::clear()
{
    /*if(rootItem_)
    {
        beginResetModel();
        delete rootItem_;
        rowCount_ = 0;
        endResetModel();
    }*/
}

void XmlModel::load(XmlItem *rootItem)
{
    if(rootItem)
    {
        //beginInsertRows(QModelIndex(), 0, rootItem_->childs().count());

        XmlItem *child;
        foreach(child, rootItem->childs())
            list_ << child;

        rowCount_ = rootItem->childs().count();

        //endInsertRows();
    }
}
