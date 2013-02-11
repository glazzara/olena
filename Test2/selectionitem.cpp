#include "selectionitem.h"

SelectionItem::SelectionItem()
{
    isPress = false;
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(3);
    QBrush brush;
    brush.setStyle(Qt::Dense4Pattern);
    brush.setColor(QColor::fromRgb(100, 200, 60, 100));
    setBrush(brush);
}

SelectionItem::SelectionItem(QRectF rectangle)
{
    setRect(rectangle);
    isPress = false;
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(3);
    QBrush brush;
    brush.setStyle(Qt::Dense4Pattern);
    brush.setColor(QColor::fromRgb(100, 200, 60, 100));
    setBrush(brush);
}

SelectionItem::SelectionItem(qreal x, qreal y, qreal width, qreal height)
{
    setRect(x, y, width, height);
    isPress = false;
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(3);
    QBrush brush;
    brush.setStyle(Qt::Dense4Pattern);
    brush.setColor(QColor::fromRgb(100, 200, 60, 100));
    setBrush(brush);
}

void SelectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPress)
    {
        isPress = true;
        pressPoint = event->pos();
    }
}

void SelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isPress)
    {
        setRect(QRectF(event->pos(), QSizeF(pressPoint.x()-event->pos().x(), pressPoint.y()-event->pos().y())));
        update();
    }
}

void SelectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPress = false;
        setRect(QRectF(event->pos(), QSizeF(pressPoint.x()-event->pos().x(), pressPoint.y()-event->pos().y())));
        update();
    }
}
