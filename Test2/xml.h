#ifndef XML_H
#define XML_H

#include <QString>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QDomElement>
#include <QTextEdit>
#include "region.h"

class Xml
{
    public:
        Xml();
        static QVector<QGraphicsItem *> graphicsItems(const QString &filename, QTextEdit *debug);

    private:
        static void graphicsTypoRegion(const QDomElement &element, QGraphicsItem *parent, const QPoint &xPos);
        static void graphicsLineRegion(const QDomElement &element, QGraphicsItem *parent);
        static void graphicsTextRegion(const QDomElement &element, QVector<QGraphicsItem *>& items, QTextEdit *debug);
};

#endif // XML_H
