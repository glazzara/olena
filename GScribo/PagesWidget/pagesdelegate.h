#ifndef PAGESDELEGATE_H
#define PAGESDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class PagesDelegate :
        public QStyledItemDelegate
{
    public:
        explicit PagesDelegate(QObject *parent = 0);
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private:
        QPen pen_;
};

#endif // PAGESDELEGATE_H
