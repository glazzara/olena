#ifndef XML_H
#define XML_H

#include <QString>
#include <QPainterPath>
#include <QDomElement>
#include "graphicsregion.h"

class Xml
{
    public:
        Xml();
        static QMap<QString, GraphicsRegion *> mapItems(const QString &filename);

    private:
        static void graphicsTypoRegion(const QDomElement &element, const QString &typologicalLine, const QPoint &xPos, QMap<QString, GraphicsRegion *> *map);
        static void graphicsLineRegion(const QDomElement &element, QMap<QString, GraphicsRegion *> *map);
        static void graphicsTextRegion(const QDomElement &element, QMap<QString, GraphicsRegion *> *map);
};

#endif // XML_H
