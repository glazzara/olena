#ifndef XML_H
#define XML_H

#include <QGraphicsItem>
#include <QDomElement>
#include <QTextEdit>
#include <climits>
#include <QString>
#include <QFile>

#include "polygonitem.h"
#include "region.h"
#include "region.h"
#include "scene.h"

struct Node
{
    QDomElement root;
    QDomElement
};

class Xml:
        QObject
{
    public:
        Xml();
        static void parseItems(const QString &filename, Scene *scene);

    private:
        void graphicsRegion(const QDomElement& element, const GraphicRegion::Data& data, Scene *scene);
        void graphicsTypoRegion(const QDomElement& element, const QPoint& xPos, Scene *scene);
        void graphicsLineRegion(const QDomElement& element, Scene *scene);
        void graphicsTextRegion(const QDomElement& element, Scene *scene);

        QDomElement root;
};

#endif // XML_H
