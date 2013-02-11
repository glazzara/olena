#include "listmodel.h"

ListModel::ListModel(QObject *parent):
        QAbstractListModel(parent)
{
    currentRow_ = -1;
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
    int row = rowCount();

    beginInsertRows(QModelIndex(), row, row);

    pixmaps_.insert(row, pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    filenames_.insert(row, filename);

    endInsertRows();
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
    int precRow = currentRow_;
    currentRow_ = currentRow;

    QModelIndex i = index(precRow, 0, QModelIndex());
    emit dataChanged(i, i);
    i = index(currentRow_, 0, QModelIndex());
    emit dataChanged(i, i);
}
