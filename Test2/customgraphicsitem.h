#ifndef CUSTOMGRAPHICSITEM_H
#define CUSTOMGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>

class customGraphicsItem : public QGraphicsItem
{
    public:
        customGraphicsItem(QRectF rectangle);
        //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:
        QRectF rectangle;
};

#endif // CUSTOMGRAPHICSITEM_H
