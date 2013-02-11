#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QSortFilterProxyModel>
#include <QGraphicsView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QDomElement>
#include <QHeaderView>
#include <QWidget>
#include <QLabel>

#include "Rendering/scene.h"
#include "selectionproxy.h"
#include "attributemodel.h"
#include "variantpointer.h"
#include "xmldelegate.h"
#include "xmlmodel.h"

class XmlWidget :
        public QWidget
{
        Q_OBJECT

    public:
        explicit XmlWidget(QWidget *parent = 0);

        void changeView(XmlItem *rootItem);

    private:
        QSortFilterProxyModel proxy_;
        SelectionProxy selectionProxy_;

        XmlModel model_;
        AttributeModel attributesModel_;

        QTreeView selection_;
        QTreeView property_;

    public slots:
        inline void onBeginGraphicalSelection();
        inline void onEndGraphicalSelection();

    private slots:
        void onClick(const QModelIndex& index);

    signals:
        void select(PolygonItem *polygonItem);
};

inline void XmlWidget::onBeginGraphicalSelection()
{ selectionProxy_.beginResetModel(); }

inline void XmlWidget::onEndGraphicalSelection()
{ selectionProxy_.endResetModel(); attributesModel_.reset(); selection_.expandAll(); }

#endif // XMLWIDGET_H
