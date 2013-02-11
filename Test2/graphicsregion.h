#ifndef GRAPHICSREGION_H
#define GRAPHICSREGION_H

#include <QGraphicsItem>

class GraphicsRegion :
        public QGraphicsItem
{
    public:
        GraphicsRegion();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void setPainterPath(QPainterPath *painterPath);

    private:
        QPainterPath *painterPath;
};

#endif // GRAPHICSREGION_H
