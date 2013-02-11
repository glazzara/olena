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

    protected:
        inline bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

    private:
        bool isAllDisplayed_;
};

inline bool SelectionProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{ return isAllDisplayed_ || sourceModel()->index(sourceRow, 0, sourceParent).data(filterRole()).value<XmlItem *>()->isVisible(); }

inline void SelectionProxy::beginResetModel()
{ isAllDisplayed_ = false; QSortFilterProxyModel::beginResetModel(); }

inline void SelectionProxy::endResetModel()
{ QSortFilterProxyModel::endResetModel(); }

inline void SelectionProxy::displayAll()
{ isAllDisplayed_ = true; }

inline bool SelectionProxy::isAllDisplayed()
{ return isAllDisplayed_; }

#endif // SELECTIONPROXY_H
