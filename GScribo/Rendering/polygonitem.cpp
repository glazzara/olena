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
    xmlItem_ = 0;

    selectedPen_.setStyle(Qt::SolidLine);
    unselectedPen_.setStyle(Qt::SolidLine);

    selectedPen_.setWidthF(2);
    unselectedPen_.setWidth(0);

    selectedPen_.setCapStyle(Qt::SquareCap);
    unselectedPen_.setCapStyle(Qt::SquareCap);

    selectedBrush_.setStyle(Qt::SolidPattern);
    unselectedBrush_.setStyle(Qt::SolidPattern);
}

void PolygonItem::setColor(const QColor& color)
{
    selectedPen_.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 255));
    unselectedPen_.setColor(color);

    selectedBrush_.setColor(color);
    unselectedBrush_.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 40));

    setPen(unselectedPen_);
    setBrush(unselectedBrush_);
}

void PolygonItem::setSelected(bool isSelected)
{
    if(isSelected)
    {
        isSelected_ = true;
        setPen(selectedPen_);
        setBrush(selectedBrush_);
    }

    else
    {
        isSelected_ = false;
        setPen(unselectedPen_);
        setBrush(unselectedBrush_);
    }
}
