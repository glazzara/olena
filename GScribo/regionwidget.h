#ifndef REGIONWIDGET_H
#define REGIONWIDGET_H

#include <QTreeWidget>
#include <QLabel>

#include "xml.h"

class RegionWidget :
        public QTreeWidget
{
        Q_OBJECT

    public:
        explicit RegionWidget(QWidget *parent = 0);

        inline QString filterString() const;

    private:
        QTreeWidgetItem *createRoot(const QString& text, const GraphicsRegion::Id& region, const GraphicsRegion::Id& begin, const GraphicsRegion::Id& end);
        QTreeWidgetItem *createItem(const QString& text, const GraphicsRegion::Id& region, const QColor& color = QColor::fromRgb(255, 255, 255));
        inline void fillRoot(QTreeWidgetItem *rootItem, const GraphicsRegion::Id& region);

        QString filterString_;

    private slots:
        void checkStateChanged(QTreeWidgetItem *item);

    signals:
        void checkStateChanged(const GraphicsRegion::Id& region, bool checked);
        void checkStateChanged(const QString& filterString);
};

inline void RegionWidget::fillRoot(QTreeWidgetItem *rootItem, const GraphicsRegion::Id& region)
{ GraphicsRegion::Data data = Xml::dataFromRegion(region); filterString_.append('|' + data.name); rootItem->addChild(createItem(data.name, region, data.color)); }

inline QString RegionWidget::filterString() const
{ return filterString_.right(filterString_.count()-1); }

#endif // REGIONWIDGET_H
