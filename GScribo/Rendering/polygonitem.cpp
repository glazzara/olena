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

void PolygonItem::setColor(const QColor &color)
{
    selectedPen_.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 255));
    unselectedPen_.setColor(color);

    selectedBrush_.setColor(color);
    unselectedBrush_.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 40));

    setPen(unselectedPen_);
    setBrush(unselectedBrush_);
}

bool PolygonItem::isSelected(const QRectF& rect, bool clic)
{
    if(clic)
        return (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().contains(rect.bottomRight())) &&
            shape().contains(rect.bottomRight());
    else
        return (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().intersects(rect)) &&
            shape().intersects(rect);
}

void PolygonItem::setSelected(const QRectF& rect, bool clic)
{
    if(isSelected(rect, clic))
    {
        // Select graphical and xml item.
        select();
        xmlItem_->select();
    }

    else
    {
        // Unselect graphical and xml item.
        unselect();
        xmlItem_->unselect();
    }

    // Set selection for line items.
    if(region_ == (int)GraphicRegion::Text)
    {
        QGraphicsItem *child;
        foreach(child, childItems())
            static_cast<PolygonItem *>(child)->setSelected(rect, clic);
    }
}
