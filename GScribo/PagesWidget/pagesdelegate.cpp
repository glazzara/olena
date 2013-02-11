#include "pagesdelegate.h"

PagesDelegate::PagesDelegate(QObject *parent) :
        QStyledItemDelegate(parent)
{
    pen_.setColor(QColor::fromRgb(255, 69, 0, 255));
    pen_.setWidth(2);
}

void PagesDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if(index.row() == index.data(Qt::UserRole+1).toInt())
    {
        painter->setPen(pen_);
        painter->drawRect(option.rect);
        painter->fillRect(option.rect, QColor::fromRgb(255, 69, 0, 60));
    }
}
