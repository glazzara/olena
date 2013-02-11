#ifndef SELECTIONPROXY_H
#define SELECTIONPROXY_H

#include <QSortFilterProxyModel>
#include <QDebug>

#include "xmlitem.h"

class SelectionProxy :
        public QSortFilterProxyModel
{
    public:
        explicit SelectionProxy(QObject *parent = 0) : QSortFilterProxyModel(parent) { }
        inline void beginResetModel();
        inline void endResetModel();

    protected:
        inline bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;
};

inline bool SelectionProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    return index.data(filterRole()).toBool();
}

inline void SelectionProxy::beginResetModel()
{ QSortFilterProxyModel::beginResetModel(); }

inline void SelectionProxy::endResetModel()
{ QSortFilterProxyModel::endResetModel(); }

#endif // SELECTIONPROXY_H
