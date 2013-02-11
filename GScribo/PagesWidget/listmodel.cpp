#include "listmodel.h"

ListModel::ListModel(QObject *parent):
        QAbstractListModel(parent)
{
    currentRow_ = 0;
}

QVariant ListModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DecorationRole)
        return pixmaps_[index.row()];

    if(role == Qt::UserRole)
        return filenames_[index.row()];

    if(role == Qt::UserRole+1)
        return currentRow_;

   return QVariant();
}

void ListModel::addPicture(const QString& filename, const QPixmap& pixmap)
{
    beginInsertRows(QModelIndex(), 0, 0);
    beginResetModel();

    pixmaps_.prepend(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    filenames_.prepend(filename);

    endInsertRows();
    //endResetModel();
}

void ListModel::removePixmap(const QModelIndex &parent)
{
    int row = parent.row();

    beginRemoveRows(parent, row, row);

    pixmaps_.removeAt(row);
    filenames_.removeAt(row);

    endRemoveRows();
}

void ListModel::setCurrentRow(int currentRow)
{
    beginResetModel();
    currentRow_ = currentRow;
    endResetModel();
}
