#ifndef REGIONWIDGET_H
#define REGIONWIDGET_H

#include <QTreeWidget>
#include <QLabel>

#include "regionwidgetitem.h"
#include "xml.h"

class RegionWidget :
        public QTreeWidget
{
        Q_OBJECT

    public:
        explicit RegionWidget(QWidget *parent = 0);

    private:
        RegionWidgetItem *createRoot(const QString& text, const GraphicsRegion::Id& region, const GraphicsRegion::Id& begin, const GraphicsRegion::Id& end);
        RegionWidgetItem *createItem(const QString& text, const GraphicsRegion::Id& region, const QColor& color = QColor::fromRgb(255, 255, 255));
        inline void fillRoot(RegionWidgetItem *rootItem, const GraphicsRegion::Id& region);

    private slots:
        void onItemChanged(QTreeWidgetItem *item);

    signals:
        void checkStateChanged(GraphicsRegion::Id region);
};

inline void RegionWidget::fillRoot(RegionWidgetItem *rootItem, const GraphicsRegion::Id& region)
{ GraphicsRegion::Data data = Xml::dataFromRegion(region); rootItem->addChild(createItem(data.name, region, data.color)); }

#endif // REGIONWIDGET_H
