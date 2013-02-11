#ifndef SELECTIONPROXY_H
#define SELECTIONPROXY_H

#include <QSortFilterProxyModel>
#include <QDebug>

#include "xmlitem.h"

class SelectionProxy :
        public QSortFilterProxyModel
{
    public:
        explicit SelectionProxy(QObject *parent = 0) : QSortFilterProxyModel(parent) { isAllSelected_ = true; }

        inline void beginResetModel();
        inline void endResetModel();

        inline void selectAll();
        inline bool isAllSelected();

    protected:
        inline bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

    private:
        bool isAllSelected_;
};

inline bool SelectionProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{ return isAllSelected_ || sourceModel()->index(sourceRow, 0, sourceParent).data(filterRole()).toBool(); }

inline void SelectionProxy::beginResetModel()
{ isAllSelected_ = false; QSortFilterProxyModel::beginResetModel(); }

inline void SelectionProxy::endResetModel()
{ QSortFilterProxyModel::endResetModel(); }

inline void SelectionProxy::selectAll()
{ isAllSelected_ = true; }

inline bool SelectionProxy::isAllSelected()
{ return isAllSelected_; }

#endif // SELECTIONPROXY_H
