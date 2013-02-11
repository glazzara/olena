#ifndef SELECTIONITEM_H
#define SELECTIONITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QPainter>

class SelectionItem :
        public QGraphicsRectItem
{
    public:
        SelectionItem();
        SelectionItem(qreal x, qreal y, qreal width, qreal height);
        SelectionItem(QRectF rectangle);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        QPointF pressPoint;
        bool isPress;
};

#endif // SELECTIONITEM_H
