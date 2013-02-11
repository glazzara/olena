#ifndef GRAPHICSREGION_H
#define GRAPHICSREGION_H

#include <QGraphicsItem>
#include <QPainter>

class GraphicsRegion :
        public QGraphicsItem
{
    public:
        GraphicsRegion();
        GraphicsRegion(QPainterPath *painterPath);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;
        QPainterPath shape() const;
        void setPainterPath(QPainterPath *painterPath);

    private:
        QPainterPath *painterPath;
};

#endif // GRAPHICSREGION_H
