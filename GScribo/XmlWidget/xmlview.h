#ifndef XMLVIEW_H
#define XMLVIEW_H

#include <QTreeView>

#include "xmlitem.h"
#include "xmlmodel.h"

class XmlView :
        public QTreeView
{
    public:
        explicit XmlView(XmlItem *rootItem = 0);

        void load(XmlItem *rootItem);

        inline XmlItem *rootItem() const;
        inline XmlModel *model() const;

    private:
        XmlModel *model_;
};

inline XmlModel *XmlView::model() const
{ return model_; }

#endif // XMLVIEW_H
