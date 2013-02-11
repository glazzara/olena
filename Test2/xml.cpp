#include <QFile>
#include <QTextStream>
#include <climits>
#include "xml.h"

Xml::Xml()
{
}

void Xml::graphicsTypoRegion(const QDomElement &element, const QString &typologicalName, const QPoint &xPos)
{
    GraphicsRegion *typoRegion = new GraphicsRegion();
    int yPos = element.attribute(typologicalName, "null").toInt();
    typoRegion->setData(0, QVariant::fromValue(QString::number(-yPos)));
    QPainterPath painterPath(QPoint(xPos.x(), yPos));
    painterPath.lineTo(QPoint(xPos.y(), yPos));
    typoRegion->setPainterPath(&painterPath);
}

void Xml::graphicsLineRegion(const QDomElement &element)
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
        graphicsTypoRegion(element.parentNode().toElement(), "baseline", QPoint(xMin, xMax));
        graphicsTypoRegion(element.parentNode().toElement(), "meanline", QPoint(xMin, xMax));
        graphicsLineRegion(element.nextSiblingElement("Line"));
    }
}

void Xml::graphicsTextRegion(const QDomElement &element)
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
        graphicsLineRegion(element.firstChild().nextSiblingElement("Line"));
        graphicsTextRegion(element.nextSiblingElement("TextRegion"));
    }
}

QVector<QGraphicsItem> Xml::mapItems(const QString &filename)
{
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&xmlFile);
    xmlFile.close();
    graphicsTextRegion(xml.documentElement().firstChild().nextSiblingElement("Page").firstChild().toElement());
}
