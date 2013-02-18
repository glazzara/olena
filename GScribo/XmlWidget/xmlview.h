#ifndef XMLVIEW_H
#define XMLVIEW_H

#include <QGraphicsItem>
#include <QTreeView>

#include "Rendering/regionitem.h"
#include "selectionproxy.h"
#include "xmldelegate.h"
#include "xmlitem.h"

class XmlView :
        public QTreeView
{
        Q_OBJECT

    public:
        explicit XmlView(QWidget *parent = 0);

        inline QList<XmlItem *> displayedItems() const;
        void hideAll();

        inline QList<XmlItem *> selectedItems() const;

        inline void setModel(QAbstractItemModel *model);

    protected:
        void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    private:
        QList<XmlItem *> transform(const QModelIndexList& modelIndexList) const;
        void display(QList<XmlItem *> displayedItems, bool addToDisplayedList);

        SelectionProxy proxy_;
        QList<XmlItem *> displayedItems_;

    public slots:
        inline void display(QList<XmlItem *> displayedItems);
        inline void displayOnly(QList<XmlItem *> displayedItems);

        inline void displayAll();

        inline void setFilterString(const QString& filterString);

    signals:
        void loadAttributes(const XmlAttributes& xmlAttributes);

        void select(const QList<XmlItem *>& selectedItems);
        void unselect(const QList<XmlItem *>& unselectedItems);

        void newSelection();
        void emptySelection();

        void resetSelection();
        void resetProperty();
};

inline QList<XmlItem *> XmlView::displayedItems() const
{ return displayedItems_; }

inline void XmlView::display(QList<XmlItem *> displayedItems)
{ display(displayedItems, true); }

inline void XmlView::displayOnly(QList<XmlItem *> displayedItems)
{ display(displayedItems, false); }

inline void XmlView::displayAll()
{ hideAll(); proxy_.displayAll(); }

inline QList<XmlItem *> XmlView::selectedItems() const
{ return transform(selectedIndexes()); }

inline void XmlView::setFilterString(const QString& filterString)
{ proxy_.setFilterRegExp(QRegExp(filterString, Qt::CaseSensitive)); proxy_.invalidateFilter(); }

inline void XmlView::setModel(QAbstractItemModel *model)
{ proxy_.setSourceModel(model); QTreeView::setModel(&proxy_); }

#endif // XMLVIEW_H
