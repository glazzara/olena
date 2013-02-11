#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QDomElement>
#include <QWidget>
#include <QLabel>

#include "Rendering/polygonitem.h"
#include "variantpointer.h"

class XmlWidget :
        public QWidget
{
        Q_OBJECT

    public:
        explicit XmlWidget(QWidget *parent = 0);

        void changeView(QTreeWidgetItem *treeItem);

    private:
        void fillProperty(QTreeWidgetItem *treeItem);
        void deleteItemsNode(QTreeWidgetItem *topLevelItem);

        QTreeWidget viewer;
        QTreeWidget property;

    public slots:
        void setSelectItem(QTreeWidgetItem *treeItem);
        inline void clearSelection();

    signals:
        void selectGraphicalItem(PolygonItem *polygonItem);
};

inline void XmlWidget::clearSelection()
{ viewer.reset(); property.clear(); }

#endif // XMLWIDGET_H
