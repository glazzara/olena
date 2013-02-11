#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include <QGraphicsPolygonItem>
#include <QTreeWidgetItem>
#include <QPen>

#include "XmlWidget/xmlitem.h"
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

        inline void setXmlItem(XmlItem *xmlItem);
        inline XmlItem *xmlItem() const;

        inline void loadData(const GraphicRegion::Data& data);
        inline int region() const;

        bool isSelected(const QRectF& rect, bool clic);
        void setSelected(const QRectF& rect, bool clic);
        inline void unselect();
        inline void select();

    private:
        void init();

        XmlItem *xmlItem_;
        int region_;

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

inline void PolygonItem::loadData(const GraphicRegion::Data& data)
{ region_ = data.region; setColor(data.color); setZValue(data.zValue); }

inline int PolygonItem::region() const
{ return region_; }

inline void PolygonItem::select()
{ if(pen() != selectedPen_) { setPen(selectedPen_); setBrush(selectedBrush_); } }

inline void PolygonItem::unselect()
{ if(pen() != unselectedPen_) { setPen(unselectedPen_); setBrush(unselectedBrush_);} }

#endif // POLYGONITEM_H
