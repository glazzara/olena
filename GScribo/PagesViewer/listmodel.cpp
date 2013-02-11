#include "listmodel.h"

ListModel::ListModel(QObject *parent):
        QAbstractListModel(parent)
{
}

QStringList ListModel::filenames() const
{
    return paths;
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

    // Store the original pixmap in a custom place.
    if(role == Qt::UserRole)
        return pixmaps.value(index.row());

    // Store the path of the pixmap in an other custom place.
    if(role == Qt::UserRole+1)
        return paths.value(index.row());

   return QVariant();
}

void ListModel::addPixmap(const QString& filename, const QPixmap& pixmap)
{
    int row = pixmaps.size();

    beginInsertRows(QModelIndex(), row, row);

    pixmaps.insert(row, pixmap);
    paths.insert(row, filename);

    endInsertRows();
}

void ListModel::removePixmap(const QModelIndex &parent)
{
    int row = parent.row();

    beginRemoveRows(parent, row, row);

    pixmaps.removeAt(row);
    paths.removeAt(row);

    endRemoveRows();
}
