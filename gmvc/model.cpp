#include "model.h"

Model::Model(QObject *parent) :
        QAbstractListModel(parent)
{
    this->pixmaps = pixmaps;
}

int Model::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    else
        return pixmaps.size();;
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DecorationRole)
        return QIcon(pixmaps.value(index.row()).scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    if(role == Qt::UserRole)
        return pixmaps.value(index.row());

    return QVariant();
}

void Model::addPixmap(const QPixmap& pixmap)
{
    int row = pixmaps.size();
    beginInsertRows(QModelIndex(), row, row);
    pixmaps.insert(row, pixmap);
    endInsertRows();
}
