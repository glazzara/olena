#ifndef ROOTGRAPHICSITEM_H
#define ROOTGRAPHICSITEM_H

#include <QGraphicsItem>

#include "region.h"

class RootGraphicsItem :
        public QGraphicsPolygonItem
{
    public:
        explicit RootGraphicsItem(int numberRegion)
        { childsFromRegion_.fill(QList<QGraphicsItem *>(), numberRegion); }
        ~RootGraphicsItem()
        { foreach(QList<QGraphicsItem *> list, childsFromRegion_) { foreach(QGraphicsItem *child, list) delete child; } }

        inline void addItemFrom(QGraphicsItem *graphicalItem, const GraphicsRegion::Id& region);
        inline QList<QGraphicsItem *> childsFrom(const GraphicsRegion::Id& region) const;

    private:
        QVector<QList<QGraphicsItem *> > childsFromRegion_;
};

inline QList<QGraphicsItem *> RootGraphicsItem::childsFrom(const GraphicsRegion::Id& region) const
{ return childsFromRegion_[region]; }

inline void RootGraphicsItem::addItemFrom(QGraphicsItem *graphicalItem, const GraphicsRegion::Id& region)
{ childsFromRegion_[region] << graphicalItem; }

#endif // ROOTGRAPHICSITEM_H
