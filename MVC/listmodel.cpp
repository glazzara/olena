#include "listmodel.h"

ListModel::ListModel(QStringList list, QObject *parent):
        QAbstractListModel(parent)
{
    this->list = list;
}

ListModel::~ListModel()
{
}

int ListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return list.count();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && index.row() < list.size() && role == Qt::DecorationRole)
        return list.at(index.row());
    else
        return QVariant();
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DecorationRole && orientation == Qt::Horizontal)
        return QVariant();
    else
        return QString("Row %1").arg(section);
}
