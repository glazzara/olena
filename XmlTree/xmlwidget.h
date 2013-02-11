#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QDomElement>
#include <QWidget>

class XmlWidget :
        public QWidget
{
    public:
        explicit XmlWidget(const QString& filename);
};

#endif // XMLWIDGET_H
