#ifndef REGIONWIDGETITEM_H
#define REGIONWIDGETITEM_H

#include <QTreeWidgetItem>

#include "region.h"

class RegionWidgetItem :
        public QTreeWidgetItem
{
    public:
        explicit RegionWidgetItem(const GraphicsRegion::Id& region = GraphicsRegion::None);

        inline bool isCheckStateChanged() const;
        void setCheckState(int column, Qt::CheckState state);

        inline GraphicsRegion::Id region() const;
        inline void setRegion(GraphicsRegion::Id region);

    protected:
        inline void emitDataChanged();

    private:
        GraphicsRegion::Id region_;
        bool isCheckStateChanged_;
};

inline bool RegionWidgetItem::isCheckStateChanged() const
{ return isCheckStateChanged_; }

inline void RegionWidgetItem::emitDataChanged()
{
    QTreeWidgetItem::emitDataChanged();
    isCheckStateChanged_ = false;
}

inline GraphicsRegion::Id RegionWidgetItem::region() const
{ return region_; }

inline void RegionWidgetItem::setRegion(GraphicsRegion::Id region)
{ region_ = region; }

#endif // REGIONWIDGETITEM_H
