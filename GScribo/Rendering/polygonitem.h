#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include <QGraphicsPathItem>
#include <QPen>

#include "region.h"

class PolygonItem :
        public QObject,
        public QGraphicsPolygonItem
{
        Q_OBJECT

    public:
        explicit PolygonItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
        explicit PolygonItem(const QPolygonF& path, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

        void setColor(const QColor& color);
        QColor color() const;

        inline void loadData(const GraphicRegion::Data& data);

    private:
        void init();

        QPen selectedPen;
        QPen unselectedPen;
        QBrush selectedBrush;
        QBrush unselectedBrush;

    public slots:
        void repaint(const QRectF& rect, bool clic);
};

inline void PolygonItem::loadData(const GraphicRegion::Data &data)
{
    setColor(data.color);
    setZValue(data.zValue);
}

#endif // POLYGONITEM_H
