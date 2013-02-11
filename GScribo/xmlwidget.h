#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QGraphicsView>
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
        ~XmlWidget();

        void changeView(QTreeWidgetItem *treeItem);
        void selectItem(QTreeWidgetItem *treeItem, bool fromSelection = false);

    private:
        void fillProperty(QTreeWidgetItem *treeItem);
        void deleteItemsNode(QTreeWidgetItem *topLevelItem);

        QTreeWidget viewer;
        QTreeWidget selection;
        QTreeWidget property;
        QList<QTreeWidgetItem *> selectionedItems;

    public slots:
        void selectItems(const QList<QTreeWidgetItem *>& selectionTree);

        void clear();
        void clearSelection();

    private slots:
        inline void selectTreeItem(QTreeWidgetItem *treeItem);
        inline void selectSelectedItem(QTreeWidgetItem *selectedItem);

    signals:
        void selectGraphicalItem(PolygonItem *polygonItem);
};

inline void XmlWidget::selectTreeItem(QTreeWidgetItem *treeItem)
{ selectItem(treeItem, false); }

inline void XmlWidget::selectSelectedItem(QTreeWidgetItem *selectedItem)
{ selectItem(selectedItem, true); }

#endif // XMLWIDGET_H
