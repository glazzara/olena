#include "attributemodel.h"

AttributeModel::AttributeModel(const XmlAttributes& attributes, QObject *parent) :
        QAbstractItemModel(parent)
{
    attributes_ = attributes;
}

AttributeModel::AttributeModel(const XmlAttributes& attributes, QAbstractItemModelPrivate& dd, QObject *parent) :
        QAbstractItemModel(dd, parent)
{
    attributes_ = attributes;
}

QVariant AttributeModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant AttributeModel::data(const QModelIndex& index, int role) const
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

void AttributeModel::load(const XmlAttributes& attributes)
{
    beginResetModel();

    attributes_ = attributes;

    endResetModel();
}

void AttributeModel::reset()
{
    beginResetModel();

    attributes_ = XmlAttributes();

    endResetModel();
}
