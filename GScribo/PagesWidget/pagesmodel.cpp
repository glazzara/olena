#include "pagesmodel.h"

PagesModel::PagesModel(QObject *parent):
        QAbstractListModel(parent)
{
    currentRow_ = 0;
}

QVariant PagesModel::data(const QModelIndex& index, int role) const
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

void PagesModel::addPicture(const QString& filename, const QPixmap& pixmap)
{
    beginInsertRows(QModelIndex(), 0, 0);

    pixmaps_.prepend(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    filenames_.prepend(filename);

    endInsertRows();
}

void PagesModel::removePixmap(const QModelIndex &parent)
{
    int row = parent.row();

    beginRemoveRows(parent, row, row);

    pixmaps_.removeAt(row);
    filenames_.removeAt(row);

    endRemoveRows();
}

void PagesModel::setCurrentRow(int currentRow)
{
    beginResetModel();
    currentRow_ = currentRow;
    endResetModel();
}
