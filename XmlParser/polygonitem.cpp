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
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
    setFlags(QGraphicsItem::ItemClipsToShape);
    selectedPen = new QPen(Qt::SolidLine);
    unselectedPen = new QPen(Qt::SolidLine);
    selectedPen->setWidthF(3);
    unselectedPen->setWidth(0);
    selectedPen->setCapStyle(Qt::SquareCap);
    unselectedPen->setCapStyle(Qt::SquareCap);
    selectedBrush = new QBrush(Qt::SolidPattern);
    unselectedBrush = new QBrush(Qt::SolidPattern);
    QRectF bRect(boundingRect());
    if(bRect.width() < 1)
        bRect.setWidth(1);
    if(bRect.height() < 1)
        bRect.setHeight(1);
}

void PolygonItem::setColor(const QColor &color)
{
    selectedPen->setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 200));
    unselectedPen->setColor(color);
    selectedBrush->setColor(color);
    unselectedBrush->setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 30));
    setPen(*selectedPen);
    setBrush(*selectedBrush);
}

QColor PolygonItem::color() const
{
    return selectedBrush->color();
}

void PolygonItem::repaint(const QRectF &rect, bool clic)
{
    bool sel;
    if(clic)
        sel = (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().contains(rect.bottomRight())) && shape().contains(rect.bottomRight());
    else
        sel = (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().intersects(rect)) && shape().intersects(rect);
    if(sel)
    {
        if(pen() != *selectedPen)
        {
            setPen(*selectedPen);
            setBrush(*selectedBrush);
            update(rect);
        }
    }
    else
    {
        if(pen() != *unselectedPen)
        {
            setPen(*unselectedPen);
            setBrush(*unselectedBrush);
            update(rect);
        }
    }
}
