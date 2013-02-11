#include "polygonitem.h"

PolygonItem::PolygonItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    init();
}

PolygonItem::PolygonItem(const QPolygonF &polygon, QGraphicsItem *parent, QGraphicsScene *scene):
        QGraphicsPolygonItem(polygon, parent, scene)
{
    init();
}

void PolygonItem::init()
{
    selectedPen.setStyle(Qt::SolidLine);
    unselectedPen.setStyle(Qt::SolidLine);

    selectedPen.setWidthF(2);
    unselectedPen.setWidth(0);

    selectedPen.setCapStyle(Qt::SquareCap);
    unselectedPen.setCapStyle(Qt::SquareCap);

    selectedBrush.setStyle(Qt::SolidPattern);
    unselectedBrush.setStyle(Qt::SolidPattern);
}

void PolygonItem::setColor(const QColor &color)
{
    selectedPen.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 255));
    unselectedPen.setColor(color);

    selectedBrush.setColor(color);
    unselectedBrush.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 40));

    setPen(unselectedPen);
    setBrush(unselectedBrush);
}
