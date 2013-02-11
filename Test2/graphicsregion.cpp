#include <QStyleOptionGraphicsItem>
#include "graphicsregion.h"

GraphicsRegion::GraphicsRegion()
{
}

GraphicsRegion::GraphicsRegion(QPainterPath *painterPath)
{
    this->painterPath = painterPath;
}

void GraphicsRegion::setPainterPath(QPainterPath *painterPath)
{
    this->painterPath = painterPath;
}

QRectF GraphicsRegion::boundingRect() const
{
    return painterPath->boundingRect();
}

QPainterPath GraphicsRegion::shape() const
{
    return *painterPath;
}

void GraphicsRegion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPath(*painterPath);
}
