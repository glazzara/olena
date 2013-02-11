#include "xmldelegate.h"

XmlDelegate::XmlDelegate(QObject *parent) :
        QStyledItemDelegate(parent)
{
}

void XmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

}
