#include "xml.h"
#include "region.h"

Xml::Xml()
{
}

void Xml::graphicsTypoRegion(const QDomElement &element, QGraphicsItem *parent, const QPoint &xPos)
{
    int yPos = element.attribute("baseline", "null").toInt();
    QGraphicsLineItem *baselineRegion = new QGraphicsLineItem(QLine(QPoint(xPos.x(), yPos), QPoint(xPos.y(), yPos)), parent);
    baselineRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Baseline));
    yPos = element.attribute("meanline", "null").toInt();
    QGraphicsLineItem *meanlineRegion = new QGraphicsLineItem(QLine(QPoint(xPos.x(), yPos), QPoint(xPos.y(), yPos)), parent);
    meanlineRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Meanline));
}

void Xml::graphicsLineRegion(const QDomElement &element, QGraphicsItem *parent)
{
    if(!element.isNull())
    {
        QPolygon polygonLine;
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
        QGraphicsPolygonItem *lineRegion = new QGraphicsPolygonItem(polygonLine, parent);
        lineRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Line));
        graphicsTypoRegion(element, lineRegion, QPoint(xMin, xMax));
        graphicsLineRegion(element.nextSiblingElement("Line"), parent);
    }
}

void Xml::graphicsTextRegion(const QDomElement &element, QVector<QGraphicsItem *>& items, QTextEdit *debug)
{
    if(!element.isNull())
    {
        QPolygon polygonText;
        QDomElement elt = element.firstChild().firstChild().toElement();
        while(!elt.isNull())
        {
            polygonText << QPoint(elt.attribute("x", "null").toInt(), elt.attribute("y", "null").toInt());
            elt = elt.nextSibling().toElement();
        }
        QGraphicsPolygonItem *textRegion = new QGraphicsPolygonItem(polygonText);
        textRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Text));
        items << textRegion;
        graphicsLineRegion(element.firstChild().nextSiblingElement("Line"), textRegion);
        graphicsTextRegion(element.nextSiblingElement("TextRegion"), items, debug);
    }
}

QVector<QGraphicsItem *> Xml::graphicsItems(const QString &filename, QTextEdit *debug)
{
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&xmlFile);
    xmlFile.close();
    QVector<QGraphicsItem *> items;
    graphicsTextRegion(xml.documentElement().firstChild().nextSiblingElement("Page").firstChild().toElement(), items, debug);
    return items;
}
