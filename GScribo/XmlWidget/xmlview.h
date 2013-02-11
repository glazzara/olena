#ifndef XMLVIEW_H
#define XMLVIEW_H

#include <QTreeView>

#include "xmlitem.h"
#include "xmlmodel.h"

class XmlView :
        public QTreeView
{
    public:
        explicit XmlView(XmlItem * = 0);
};

#endif // XMLVIEW_H
