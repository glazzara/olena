#ifndef SELECTIONPROXY_H
#define SELECTIONPROXY_H

#include <QSortFilterProxyModel>
#include <QDebug>

#include "xmlitem.h"

class SelectionProxy :
        public QSortFilterProxyModel
{
    public:
        explicit SelectionProxy(QObject *parent = 0) : QSortFilterProxyModel(parent) { isAllDisplayed_ = true; }

        inline void displayAll();
        inline bool isAllDisplayed();

        inline void beginResetModel();
        inline void endResetModel();
        inline void invalidateFilter();

    protected:
        inline bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

    private:
        bool isAllDisplayed_;
};

inline bool SelectionProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{ return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent) && (isAllDisplayed_ || sourceModel()->index(sourceRow, 0, sourceParent).data(Qt::UserRole+2).value<XmlItem *>()->isVisible()); }

inline void SelectionProxy::beginResetModel()
{ isAllDisplayed_ = false; QSortFilterProxyModel::beginResetModel(); }

inline void SelectionProxy::endResetModel()
{ QSortFilterProxyModel::endResetModel(); }

inline void SelectionProxy::displayAll()
{ isAllDisplayed_ = true; }

inline bool SelectionProxy::isAllDisplayed()
{ return isAllDisplayed_; }

inline void SelectionProxy::invalidateFilter()
{ QSortFilterProxyModel::invalidateFilter(); }

#endif // SELECTIONPROXY_H
