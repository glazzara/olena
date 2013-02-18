#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QSortFilterProxyModel>
#include <QTableView>
#include <QTreeWidget>
#include <QSplitter>
#include <QDomElement>
#include <QWidget>

#include "Rendering/scene.h"
#include "selectionproxy.h"
#include "attributesmodel.h"
#include "variantpointer.h"
#include "xmldelegate.h"
#include "xmlmodel.h"
#include "xmlview.h"

class XmlWidget :
        public QSplitter
{
        Q_OBJECT

    public:
        explicit XmlWidget(QWidget *parent = 0);

        inline XmlView *view();
        inline QTableView *attributes();

        void changeView(XmlItem *rootItem);

    private:
        QSortFilterProxyModel proxy_;

        XmlModel model_;
        AttributesModel attributesModel_;

        XmlView view_;
        QTableView attributes_;

    private slots:
        inline void loadAttributes(const QModelIndex& index);
};

inline XmlView * XmlWidget::view()
{ return &view_; }

inline QTableView *XmlWidget::attributes()
{ return &attributes_; }

inline void XmlWidget::loadAttributes(const QModelIndex& index)
{ attributesModel_.load(index.data(Qt::UserRole+2).value<XmlItem *>()->attributes()); }

#endif // XMLWIDGET_H
