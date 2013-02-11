#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include <QGraphicsPolygonItem>
#include <QTreeWidgetItem>
#include <QPen>

#include "variantpointer.h"
#include "region.h"

class XmlItem;

class PolygonItem :
        public QGraphicsPolygonItem
{
    public:
        explicit PolygonItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
        explicit PolygonItem(const QPolygonF& path, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

        void setColor(const QColor& color);
        inline QColor color() const;

        inline void setXmlItem(XmlItem *xmlItem);
        inline XmlItem *xmlItem() const;

        inline void loadData(const GraphicRegion::Data& data);
        inline int region() const;

        inline bool intersects(const QRectF& rect);
        inline bool contains(const QPointF& point);

        void setSelected(bool isSelected);
        inline bool isSelected();
        inline void unselect();
        inline void select();

    private:
        void init();

        XmlItem *xmlItem_;
        int region_;
        bool isSelected_;

        QPen selectedPen_;
        QPen unselectedPen_;
        QBrush selectedBrush_;
        QBrush unselectedBrush_;
};

inline void PolygonItem::setXmlItem(XmlItem *xmlItem)
{ xmlItem_ = xmlItem; }

inline XmlItem *PolygonItem::xmlItem() const
{ return xmlItem_; }

inline QColor PolygonItem::color() const
{ return selectedBrush_.color(); }

inline int PolygonItem::region() const
{ return region_; }

inline bool PolygonItem::isSelected()
{ return isSelected_; }

inline void PolygonItem::select()
{ setSelected(true); }

inline void PolygonItem::unselect()
{ setSelected(false); }

inline void PolygonItem::loadData(const GraphicRegion::Data& data)
{ region_ = data.region; setColor(data.color); setZValue(data.zValue); }

inline bool PolygonItem::contains(const QPointF& point)
{ return (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().contains(point)) && shape().contains(point); }

inline bool PolygonItem::intersects(const QRectF& rect)
{ return (boundingRect().width() == 0 || boundingRect().height() == 0 || boundingRect().intersects(rect)) && shape().intersects(rect); }

#endif // POLYGONITEM_H
