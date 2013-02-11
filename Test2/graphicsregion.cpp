#include "graphicsregion.h"

GraphicsRegion::GraphicsRegion()
{
}

void GraphicsRegion::setPainterPath(QPainterPath *painterPath)
{
    this->painterPath = painterPath;
}

void GraphicsRegion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
