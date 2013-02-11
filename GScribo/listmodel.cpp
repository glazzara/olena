#include "listmodel.h"

ListModel::ListModel(QStringList stringList, QObject *parent):
        QAbstractListModel(parent)
{
    this->stringList = stringList;
}

ListModel::~ListModel()
{
}

int ListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return stringList.count();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && stringList.size() < index.row() && (role == Qt::DisplayRole || role == Qt::EditRole))
        return stringList.at(index.row());
    else
        return QVariant();
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return QVariant();
    else
        return QString("Row %1").arg(section);
}

/*Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    else
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::DisplayRole)
    {
        stringList.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    else
        return false;
}*/
