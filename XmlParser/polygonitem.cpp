#include "polygonitem.h"

PolygonItem::PolygonItem(QGraphicsItem *parent, QGraphicsScene *scene):
        QGraphicsPolygonItem(parent, scene)
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
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemClipsToShape);
    selectedPen = new QPen(Qt::SolidLine);
    unselectedPen = new QPen(Qt::SolidLine);
    selectedPen->setWidthF(20);
    unselectedPen->setWidth(0);
    selectedPen->setCapStyle(Qt::SquareCap);
    unselectedPen->setCapStyle(Qt::SquareCap);
    selectedBrush = new QBrush(Qt::SolidPattern);
    unselectedBrush = new QBrush(Qt::SolidPattern);
}

void PolygonItem::setColor(const QColor &color)
{
    selectedPen->setColor(color);
    unselectedPen->setColor(color);
    selectedBrush->setColor(color);
    unselectedBrush->setColor(color);
    setPen(*selectedPen);
    setBrush(*selectedBrush);
}

QColor PolygonItem::color() const
{
    return selectedBrush->color();
}

void PolygonItem::repaint(const QRectF &rect)
{
    if(isSelected())
    {
        if(pen() != *selectedPen)
        {
            setPen(*selectedPen);
            setBrush(*selectedBrush);
        }
        else
            return;
    }
    else
    {
        if(pen() != *unselectedPen)
        {
            setPen(*unselectedPen);
            setBrush(*unselectedBrush);
        }
        else
            return;
    }
    update(rect);
}
