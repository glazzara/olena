#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QSortFilterProxyModel>
#include <QTreeWidgetItem>
#include <QDomElement>
#include <QWidget>

#include "xmldelegate.h"
#include "xmlmodel.h"
#include "xmlitem.h"

class XmlWidget :
        public QWidget
{
    public:
        explicit XmlWidget(XmlItem *item = 0);

        QTreeView tree;

    private:
        QSortFilterProxyModel proxy;
};

#endif // XMLWIDGET_H
