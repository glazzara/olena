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
    selectedPen.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 200));
    unselectedPen.setColor(color);

    selectedBrush.setColor(color);
    unselectedBrush.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 30));

    setPen(selectedPen);
    setBrush(selectedBrush);
}

QColor PolygonItem::color() const
{
    return selectedBrush.color();
}

void PolygonItem::repaint(const QRectF& rect, bool clic)
{
    bool sel;

    // For optimization, do first an intersection by bounding rectangle beetween items and selection and then an intersection by shape.
    if(clic)
        sel = (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().contains(rect.bottomRight())) && shape().contains(rect.bottomRight());
    else
        sel = (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().intersects(rect)) && shape().intersects(rect);

    // Change items brush and pen if it's selectionned or not.
    if(sel)
    {
        if(pen() != selectedPen)
        {
            setPen(selectedPen);
            setBrush(selectedBrush);
            update(rect);
        }
    }
    else
    {
        if(pen() != unselectedPen)
        {
            setPen(unselectedPen);
            setBrush(unselectedBrush);
            update(rect);
        }
    }
}
