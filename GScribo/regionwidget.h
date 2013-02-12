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
        QTreeWidgetItem *createItem(const GraphicsRegion::Id& region);

        QString filterString_;

    private slots:
        void checkStateChanged(QTreeWidgetItem *item);

    signals:
        void checkStateChanged(const GraphicsRegion::Id& region, bool checked);
        void checkStateChanged(const QString& filterString);
};

inline QString RegionWidget::filterString() const
{ return filterString_.right(filterString_.count()-1); }

#endif // REGIONWIDGET_H
