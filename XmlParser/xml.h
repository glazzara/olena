#ifndef XML_H
#define XML_H

#include <QString>
#include <QGraphicsItem>
#include <QDomElement>
#include <QFile>
#include <climits>
#include <QTextEdit>
#include "polygonitem.h"
#include "region.h"
#include "scene.h"

class Xml:
        QObject
{
    public:
        Xml();
        static void parseItems(const QString &filename, Scene *scene);

    private:
        static void graphicsRegion(const QDomElement& element, const GraphicRegion::Data& data, Scene *scene);
        static void graphicsTypoRegion(const QDomElement& element, const QPoint& xPos, Scene *scene);
        static void graphicsLineRegion(const QDomElement& element, Scene *scene);
        static void graphicsTextRegion(const QDomElement& element, Scene *scene);
};

#endif // XML_H
