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
#include "xmlview.h"

class XmlWidget :
        public QWidget
{
        Q_OBJECT

    public:
        explicit XmlWidget(QWidget *parent = 0);

        inline XmlView *view();
        inline QTreeView *property();

        void changeView(XmlItem *rootItem);

    private:
        QSortFilterProxyModel proxy_;
        SelectionProxy proxy2_;

        XmlModel model_;
        AttributeModel attributesModel_;

        XmlView view_;
        QTreeView property_;
};

inline XmlView *XmlWidget::view()
{ return &view_; }

inline QTreeView *XmlWidget::property()
{ return &property_; }

#endif // XMLWIDGET_H
