#include "listmodel.h"

ListModel::ListModel(QObject *parent):
        QAbstractListModel(parent)
{
}

int ListModel::rowCount(const QModelIndex&) const
{
    return pixmaps.size();
}

QVariant ListModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DecorationRole)
        return QIcon(pixmaps.value(index.row()).scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));

   return QVariant();
}

void ListModel::addPixmap(const QPixmap& pixmap)
{
    int row = pixmaps.size();
    beginInsertRows(QModelIndex(), row, row);
    pixmaps.insert(row, pixmap);
    endInsertRows();
}
