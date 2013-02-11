#ifndef ATTRIBUTESDELEGATE_H
#define ATTRIBUTESDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class AttributesDelegate :
        public QStyledItemDelegate
{
    public:
        explicit AttributesDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) { }

        inline void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

inline void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->drawText(option.rect, painter.index.column() == 0 ? index.data(Qt::UserRole+1).toString() : index.data(Qt::UserRole+2).toString());
}

#endif // ATTRIBUTESDELEGATE_H
