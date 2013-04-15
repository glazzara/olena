#include "attributesmodel.h"

AttributesModel::AttributesModel(const XmlAttributes& attributes, QObject *parent) :
        QAbstractItemModel(parent)
{
    attributes_ = attributes;
}

AttributesModel::AttributesModel(const XmlAttributes& attributes, QAbstractItemModelPrivate& dd, QObject *parent) :
        QAbstractItemModel(dd, parent)
{
    attributes_ = attributes;
}

QVariant AttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return tr("Name");

            case 1:
                return tr("Value");

            default:
                return QVariant();
        }
    }

    return QVariant();
}

QVariant AttributesModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role)
    {
        case Qt::UserRole:
            return attributes_.names()[index.row()];

        case Qt::UserRole+1:
            return attributes_.values()[index.row()];

        default:
            return QVariant();
    }
}

void AttributesModel::load(const XmlAttributes& attributes)
{
    beginResetModel();

    attributes_ = attributes;

    endResetModel();
}

void AttributesModel::reset()
{
    beginResetModel();

    attributes_ = XmlAttributes();

    endResetModel();
}
