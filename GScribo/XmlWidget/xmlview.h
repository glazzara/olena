#ifndef XMLVIEW_H
#define XMLVIEW_H

#include <QGraphicsItem>
#include <QTreeView>

#include "Rendering/polygonitem.h"
#include "selectionproxy.h"
#include "xmldelegate.h"
#include "xmlitem.h"

class XmlView :
        public QTreeView
{
        Q_OBJECT

    public:
        explicit XmlView(QWidget *parent = 0);

        inline QList<XmlItem *> visibleItems() const;
        void hideVisibles();

        inline void setModel(QAbstractItemModel *model);

    protected:
        void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    private:
        QList<XmlItem *> visibleItems_;
        SelectionProxy proxy_;

    public slots:
        inline void onBeginGraphicalSelection();
        void onEndGraphicalSelection(const QList<QGraphicsItem *>& selectedItems);

    signals:
        void selection(const QList<PolygonItem *>& selectedItems, bool addToSelection);
        void resetGraphicalSelection();
        void resetAttributes();
};

inline QList<XmlItem *> XmlView::visibleItems() const
{ return visibleItems_; }

inline void XmlView::onBeginGraphicalSelection()
{ proxy_.beginResetModel(); }

inline void XmlView::setModel(QAbstractItemModel *model)
{ proxy_.setSourceModel(model); QTreeView::setModel(&proxy_); }

#endif // XMLVIEW_H
