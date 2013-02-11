#ifndef ABSTRACTREGIONITEM_H
#define ABSTRACTREGIONITEM_H

#include <QGraphicsItem>

class AbstractRegionItem :
        public QGraphicsItem
{
    public:
        explicit AbstractRegionItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
};

#endif // ABSTRACTREGIONITEM_H
