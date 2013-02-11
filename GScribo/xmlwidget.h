#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QDomElement>
#include <QWidget>
#include <QLabel>

#include "variantpointer.h"

class XmlWidget :
        public QWidget
{
        Q_OBJECT

    public:
        explicit XmlWidget(QWidget *parent = 0);

        inline void changeView(QTreeWidgetItem *treeItem);

    private:
        QTreeWidget viewer;
        QTreeWidget property;

    public slots:
        void fillProperty(QTreeWidgetItem *treeItem);
        inline void selectItem(QTreeWidgetItem *item);
        inline void clearSelection();
};

inline void XmlWidget::changeView(QTreeWidgetItem *treeItem)
{ viewer.takeTopLevelItem(0); viewer.reset(); if(treeItem != 0) viewer.addTopLevelItem(treeItem); }

inline void XmlWidget::selectItem(QTreeWidgetItem *item)
{ viewer.setCurrentItem(item, 0); item->setExpanded(true); }

inline void XmlWidget::clearSelection()
{ viewer.reset(); property.clear(); }

#endif // XMLWIDGET_H
