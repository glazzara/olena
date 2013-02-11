#include "selection.h"

Selection::Selection(QGraphicsScene *scene):
    QGraphicsRectItem(0, scene)
{
    init();
}

Selection::Selection(const QRectF &rect, QGraphicsScene *scene):
    QGraphicsRectItem(rect, 0, scene)
{
    init();
}

Selection::Selection(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene):
    QGraphicsRectItem(x, y, width, height, 0, scene)
{
    init();
}

void Selection::init()
{
    setZValue(INT_MAX);
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(1);
    setPen(pen);
    QBrush brush;
    brush.setStyle(Qt::Dense4Pattern);
    brush.setColor(QColor::fromRgb(100, 200, 60, 100));
    setBrush(brush);
}

void Selection::changeShape()
{
    ensureVisible(rect());
    prepareGeometryChange();
    update(rect());
}
