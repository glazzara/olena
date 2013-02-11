#ifndef SELECTIONITEM_H
#define SELECTIONITEM_H

#include <QGraphicsRectItem>
#include <QPen>
#include <climits>


class SelectionItem :
        public QGraphicsRectItem
{
    public:
      SelectionItem();
      explicit SelectionItem(qreal x, qreal y, qreal width, qreal height);
      explicit SelectionItem(QRectF rectangle);
      void changeShape(QRectF rectangle);
};

#endif // SELECTIONITEM_H
