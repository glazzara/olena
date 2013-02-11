#include "xmldelegate.h"

XmlDelegate::XmlDelegate(QObject *parent) :
        QStyledItemDelegate(parent)
{
}

void XmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{ QStyledItemDelegate::paint(painter, option, index); painter->drawText(option.rect, index.data(Qt::UserRole).toString()); }
