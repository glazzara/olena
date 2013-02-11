#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include <QGraphicsPolygonItem>
#include <QTreeWidgetItem>
#include <QPen>

#include "variantpointer.h"
#include "region.h"

class PolygonItem :
        public QGraphicsPolygonItem
{
    public:
        explicit PolygonItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
        explicit PolygonItem(const QPolygonF& path, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

        void setColor(const QColor& color);
        inline QColor color() const;

        inline void loadData(const GraphicRegion::Data& data);

        inline bool isSelected(const QRectF& rect, bool clic);
        inline void unselect();
        inline void select();
        inline void setSelected(bool selected);

    private:
        void init();

        QPen selectedPen;
        QPen unselectedPen;
        QBrush selectedBrush;
        QBrush unselectedBrush;
};

inline QColor PolygonItem::color() const
{ return selectedBrush.color(); }

inline void PolygonItem::loadData(const GraphicRegion::Data &data)
{ setData(0, data.region); setColor(data.color); setZValue(data.zValue); }

inline bool PolygonItem::isSelected(const QRectF &rect, bool clic)
{
    if(clic)
        return (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().contains(rect.bottomRight())) && shape().contains(rect.bottomRight());
    else
        return (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().intersects(rect)) && shape().intersects(rect);
}

inline void PolygonItem::select()
{ if(pen() != selectedPen) { setPen(selectedPen); setBrush(selectedBrush); } }

inline void PolygonItem::unselect()
{ if(pen() != unselectedPen) { setPen(unselectedPen); setBrush(unselectedBrush); } }

inline void PolygonItem::setSelected(bool selected)
{ if(selected) select(); else unselect(); }

#endif // POLYGONITEM_H
