#ifndef TREEDELEGATE_H
#define TREEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

#include "xmlitem.h"

class TreeDelegate :
        public QStyledItemDelegate
{
    public:
        explicit TreeDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) { }

        inline void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

inline void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{ QStyledItemDelegate::paint(painter, option, index); painter->drawText(option.rect, index.data(Qt::UserRole).toString()); }

#endif // TREEDELEGATE_H
