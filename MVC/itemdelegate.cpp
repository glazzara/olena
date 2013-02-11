#include "itemdelegate.h"

ItemDelegate::ItemDelegate(QWidget *parent):
        QAbstractItemDelegate(parent)
{
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString path = index.model()->data(index, Qt::DecorationRole).toString();
    QPixmap pixmap(path);

    painter->drawPixmap(0, index.row() * 210, 200, 200, pixmap);
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(200, 200);
}
