#include <QFile>
#include <QTextStream>
#include <climits>
#include "xml.h"

Xml::Xml()
{
}

void Xml::graphicsTypoRegion(const QDomElement &element, const QString &typologicalName, const QPoint &xPos, QMap<QString, GraphicsRegion *> *map)
{
    GraphicsRegion *typoRegion = new GraphicsRegion();
    int yPos = element.attribute(typologicalName, "null").toInt();
    typoRegion->setData(0, QVariant::fromValue(QString::number(-yPos)));
    QPainterPath painterPath(QPoint(xPos.x(), yPos));
    painterPath.lineTo(QPoint(xPos.y(), yPos));
    typoRegion->setPainterPath(&painterPath);
    map->insert(QString::number(-yPos), typoRegion);
}

void Xml::graphicsLineRegion(const QDomElement &element, QMap<QString, GraphicsRegion *> *map)
{
    if(!element.isNull())
    {
        QPolygon polygonLine;
        GraphicsRegion *lineRegion = new GraphicsRegion();
        QString id = element.attribute("id", "null");
        lineRegion->setData(0, QVariant::fromValue(id));
        QDomElement elt = element.firstChild().firstChild().toElement();
        int xMin = INT_MAX;
        int xMax = -1;
        int x, y;
        while(!elt.isNull())
        {
            x = elt.attribute("x", "null").toInt();
            y = elt.attribute("y", "null").toInt();
            if (xMin > x)
                xMin = x;
            if (xMax < x)
                xMax = x;
            polygonLine << QPoint(x, y);
            elt = elt.nextSibling().toElement();
        }
        QPainterPath painterPath;
        painterPath.addPolygon(polygonLine);
        map->insert(id, lineRegion);
        graphicsTypoRegion(element.parentNode().toElement(), "baseline", QPoint(xMin, xMax), map);
        graphicsTypoRegion(element.parentNode().toElement(), "meanline", QPoint(xMin, xMax), map);
        graphicsLineRegion(element.nextSiblingElement("Line"), map);
    }
}

void Xml::graphicsTextRegion(const QDomElement &element, QMap<QString, GraphicsRegion *> *map)
{
    if(!element.isNull())
    {
        QPolygon polygonText;
        GraphicsRegion *textRegion = new GraphicsRegion();
        QString id = element.attribute("id", "null");
        textRegion->setData(0, QVariant::fromValue(id));
        QDomElement elt = element.firstChild().firstChild().toElement();
        while(!elt.isNull())
        {
            polygonText << QPoint(elt.attribute("x", "null").toInt(), elt.attribute("y", "null").toInt());
            elt = elt.nextSibling().toElement();
        }
        QPainterPath painterPath;
        painterPath.addPolygon(polygonText);
        map->insert(id, textRegion);
        graphicsLineRegion(element.firstChild().nextSiblingElement("Line"), map);
        graphicsTextRegion(element.nextSiblingElement("TextRegion"), map);
    }
}

QMap<QString, GraphicsRegion *> Xml::mapItems(const QString &filename)
{
    QMap<QString, GraphicsRegion *> *map = new QMap<QString, GraphicsRegion *>();
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&xmlFile);
    xmlFile.close();
    QDomElement elt = xml.documentElement().firstChild().nextSiblingElement("Page");
    //int width = elt.attribute("imageWidth", "null").toInt();
    //int height = elt.attribute("imageHeight", "null").toInt();
    elt = elt.firstChild().toElement();
    graphicsTextRegion(elt, map);
    return *map;
}
