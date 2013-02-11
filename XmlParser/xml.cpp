#include "xml.h"
#include "region.h"

Xml::Xml()
{
}

void Xml::graphicsTypoRegion(const QDomElement &element, const QPoint &xPos, Scene *scene)
{
    int yPos = element.attribute("baseline", "null").toInt();
    QGraphicsLineItem *baselineRegion = new QGraphicsLineItem(QLine(QPoint(xPos.x(), yPos), QPoint(xPos.y(), yPos)), 0, scene);
    baselineRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Baseline));
    yPos = element.attribute("meanline", "null").toInt();
    QGraphicsLineItem *meanlineRegion = new QGraphicsLineItem(QLine(QPoint(xPos.x(), yPos), QPoint(xPos.y(), yPos)), 0, scene);
    meanlineRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Meanline));
}

void Xml::graphicsLineRegion(const QDomElement &element, Scene *scene)
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
        PolygonItem *lineRegion = new PolygonItem(polygonLine, 0, scene);
        lineRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Line));
        lineRegion->setColor(QColor::fromRgb(255, 0, 0, 80));
        lineRegion->setZValue(2);
        connect(scene, SIGNAL(repaintItems(QRectF)), lineRegion, SLOT(repaint(QRectF)));
        graphicsTypoRegion(element, QPoint(xMin, xMax), scene);
        graphicsLineRegion(element.nextSiblingElement("Line"), scene);
    }
}

void Xml::graphicsTextRegion(const QDomElement &element, Scene *scene)
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
        PolygonItem *textRegion = new PolygonItem(polygonText, 0, scene);
        textRegion->setColor(QColor::fromRgb(0, 100, 0, 80));
        textRegion->setData(0, QVariant::fromValue((int)GraphicsRegion::Text));
        textRegion->setZValue(3);
        connect(scene, SIGNAL(repaintItems(QRectF)), textRegion, SLOT(repaint(QRectF)));
        graphicsLineRegion(element.firstChild().nextSiblingElement("Line"), scene);
        graphicsTextRegion(element.nextSiblingElement("TextRegion"), scene);
    }
}

void Xml::parseItems(const QString &filename, Scene *scene)
{
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&xmlFile);
    xmlFile.close();
    QVector<QGraphicsItem *> items;
    graphicsTextRegion(xml.documentElement().firstChild().nextSiblingElement("Page").firstChild().toElement(), scene);
}
