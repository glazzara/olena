#ifndef XMLPROXY_H
#define XMLPROXY_H

#include <QSortFilterProxyModel>

class XmlProxy :
        QSortFilterProxyModel
{
    public:
        explicit XmlProxy(QObject *parent = 0);
};

#endif // XMLPROXY_H
