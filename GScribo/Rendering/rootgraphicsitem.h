#ifndef ROOTGRAPHICSITEM_H
#define ROOTGRAPHICSITEM_H

#include <QGraphicsItem>

#include "region.h"

class RootGraphicsItem :
        public QGraphicsPolygonItem
{
    public:
        explicit RootGraphicsItem(int numberRegion) { childsFromRegion_.fill(QList<QGraphicsItem *>(), numberRegion); }

        inline QList<QGraphicsItem *> childsFrom(GraphicsRegion::Id region) const;
        inline void addItemFrom(QGraphicsItem *graphicalItem, GraphicsRegion::Id region);

    private:
        QVector<QList<QGraphicsItem *> > childsFromRegion_;
};

inline QList<QGraphicsItem *> RootGraphicsItem::childsFrom(GraphicsRegion::Id region) const
{ return childsFromRegion_[region]; }

inline void RootGraphicsItem::addItemFrom(QGraphicsItem *graphicalItem, GraphicsRegion::Id region)
{ childsFromRegion_[region] << graphicalItem; }

#endif // ROOTGRAPHICSITEM_H
