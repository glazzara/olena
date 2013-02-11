#include "selectionitem.h"

SelectionItem::SelectionItem()
{
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(1);
    setPen(pen);
    QBrush brush;
    brush.setStyle(Qt::Dense4Pattern);
    brush.setColor(QColor::fromRgb(100, 200, 60, 100));
    setBrush(brush);
    setZValue(INT_MAX);
}

SelectionItem::SelectionItem(QRectF rectangle)
{
    setRect(rectangle);
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(1);
    setPen(pen);
    QBrush brush;
    brush.setStyle(Qt::Dense4Pattern);
    brush.setColor(QColor::fromRgb(100, 200, 60, 100));
    setBrush(brush);
    setZValue(INT_MAX);
}

SelectionItem::SelectionItem(qreal x, qreal y, qreal width, qreal height)
{
    setRect(x, y, width, height);
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(1);
    setPen(pen);
    QBrush brush;
    brush.setStyle(Qt::Dense4Pattern);
    brush.setColor(QColor::fromRgb(100, 200, 60, 100));
    setBrush(brush);
    setZValue(INT_MAX);
}

void SelectionItem::changeShape(QRectF rectangle)
{
    setRect(rectangle);
    prepareGeometryChange();
    update(rectangle);
}
