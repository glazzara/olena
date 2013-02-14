#ifndef RegionITEM_H
#define RegionITEM_H

#include <QGraphicsPolygonItem>
#include <QTreeWidgetItem>
#include <QPen>

#include "variantpointer.h"
#include "region.h"

class XmlItem;

class RegionItem :
        public QGraphicsPolygonItem
{
    public:
        explicit RegionItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
        explicit RegionItem(const QPolygonF& path, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

        void setColor(const QColor& color);
        inline QColor color() const;

        inline void setXmlItem(XmlItem *xmlItem);
        inline XmlItem *xmlItem() const;

        inline void loadData(const GraphicsRegion::Data& data);
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

inline void RegionItem::setXmlItem(XmlItem *xmlItem)
{ xmlItem_ = xmlItem; }

inline XmlItem *RegionItem::xmlItem() const
{ return xmlItem_; }

inline QColor RegionItem::color() const
{ return selectedBrush_.color(); }

inline int RegionItem::region() const
{ return region_; }

inline bool RegionItem::isSelected()
{ return isSelected_; }

inline void RegionItem::select()
{ setSelected(true); }

inline void RegionItem::unselect()
{ setSelected(false); }

inline void RegionItem::loadData(const GraphicsRegion::Data& data)
{ region_ = data.region; setColor(data.color); setZValue(data.zValue); }

inline bool RegionItem::contains(const QPointF& point)
{ return (boundingRect().width() * boundingRect().height() == 0 || boundingRect().contains(point)) && shape().contains(point); }

inline bool RegionItem::intersects(const QRectF& rect)
{ return (boundingRect().width() * boundingRect().height() == 0 || boundingRect().intersects(rect)) && shape().intersects(rect); }

#endif // RegionITEM_H
