#include "xml.h"

Xml::Xml(const QString& filename)
{
    filename_ = filename;

    load(filename);
}

QString Xml::getPath(const QString& filename)
{
    Configs *const configs = Configs::getInstance();

    QString xmlPath = filename;

    // Get xml filename from image path.
    xmlPath.remove(0, xmlPath.lastIndexOf('/')+1);
    int pos = xmlPath.lastIndexOf('.');
    xmlPath.remove(pos, xmlPath.length()-pos);
    xmlPath += "_gui.xml";

    return configs->generalSaveXmlCustomDirPath() + "/" + xmlPath;
}

void Xml::load(const QString& filename)
{
    if(filename.isEmpty())
    {
        xmlItem_ = 0;
        graphicsItem_ = 0;
        return;
    }

    // Reset graphic and tree items;
    xmlItem_ = new XmlItem;
    graphicsItem_ = new RootGraphicsItem(13);

    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);

    // Fill new xml tree.
    xml_.clear();
    xml_.setContent(&xmlFile);

    xmlFile.close();

    QDomElement root = xml_.documentElement();
    xmlItem_->load(root);
    xmlItem_->show();

    root = root.firstChild().toElement();
    XmlItem *parentTreeItem = init(root, xmlItem_);

    // Run through the xml file structure by structure.
    root = root.nextSibling().firstChild().toElement();
    processNode(root, datas_[0], parentTreeItem);
    for(int i = 2; i < 11; i++)
        processNode(root.nextSiblingElement(datas_[i].name), datas_[i], parentTreeItem);
}

XmlItem *Xml::init(const QDomElement& root, XmlItem *rootTreeItem)
{
    XmlItem *metadataItem = new XmlItem(root, rootTreeItem);

    XmlItem *contentItem = 0;

    // Run through "METADATA" node and subnodes.
    QDomElement node = root.firstChild().toElement();
    while(!node.isNull())
    {
        contentItem = new XmlItem(node, metadataItem, contentItem);
        node = node.nextSibling().toElement();
    }

    // Add "PAGE" root node and return it.
    metadataItem = new XmlItem(root.nextSibling().toElement(), rootTreeItem, metadataItem);
    metadataItem->show();

    return metadataItem;
}

void Xml::processNode(const QDomElement& root, const GraphicsRegion::Data& data, XmlItem *pageItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        XmlItem *regionItem = new XmlItem(root, pageItem);

        QDomElement node = root.firstChild().toElement();
        XmlItem *coordsItem = new XmlItem(node, regionItem);

        QPolygon polygon;
        XmlItem *pointItem = 0;
        QString x, y;

        // Run through all points data.
        node = node.firstChild().toElement();
        while(!node.isNull())
        {
            x = node.attribute("x", "null");
            y = node.attribute("y", "null");

            pointItem = new XmlItem(node, coordsItem, pointItem);

            polygon << QPoint(x.toInt(), y.toInt());
            node = node.nextSibling().toElement();
        }

        // Create the graphics item from data structure.
        RegionItem *graphicsItem = new RegionItem(polygon, graphicsItem_);
        graphicsItem->loadData(data);
        graphicsItem_->addItemFrom(graphicsItem, data.region);

        // Store xml item and graphics item in each object for selection.
        regionItem->setRegionItem(graphicsItem);
        graphicsItem->setXmlItem(regionItem);

        if(data.region == GraphicsRegion::TextRegion)
            processLineNode(root.firstChild().nextSiblingElement("Line"), coordsItem);

        // Run through all nodes from the same data structure recursively.
        processNode(root.nextSiblingElement(data.name), data, pageItem);
    }
}

void Xml::processLineNode(const QDomElement& root, XmlItem *precItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        XmlItem *lineItem = new XmlItem(root, precItem->parent(), precItem);

        QDomElement node = root.firstChild().toElement();
        XmlItem *coordsItem = new XmlItem(node, lineItem);

        QPolygon polygon;
        XmlItem *pointItem = 0;

        QString xs, ys;
        int x, y;

        int xMin = INT_MAX;
        int xMax = INT_MIN;

        // Run through all points data.
        node = node.firstChild().toElement();
        while(!node.isNull())
        {
            xs = node.attribute("x", "null");
            ys = node.attribute("y", "null");

            pointItem = new XmlItem(node, coordsItem, pointItem);

            x = xs.toInt();
            y = ys.toInt();

            // Get beginning and end of line.
            if(xMin > x)
                xMin = x;
            if(xMax < x)
                xMax = x;

            polygon << QPoint(x, y);
            node = node.nextSibling().toElement();
        }

        // Create the graphics item from data structure.
        RegionItem *graphicsItem = new RegionItem(polygon, graphicsItem_);
        graphicsItem->loadData(datas_[1]);
        graphicsItem_->addItemFrom(graphicsItem, datas_[1].region);

        // Store xml item and graphics item in each object for selection.
        lineItem->setRegionItem(graphicsItem);
        graphicsItem->setXmlItem(lineItem);

        // Get meanline and baseline.
        processTypoNode(root, QPoint(xMin, xMax));
        // Run through all line nodes recursively.
        processLineNode(root.nextSiblingElement(datas_[1].name), lineItem);
    }
}

void Xml::processTypoNode(const QDomElement& root, const QPoint& xPoint)
{
    int yPos = root.attribute("baseline", "null").toInt();
    QGraphicsLineItem *baselineItem = new QGraphicsLineItem(QLine(QPoint(xPoint.x(), yPos), QPoint(xPoint.y(), yPos)));
    baselineItem->setZValue(datas_[11].zValue);
    graphicsItem_->addItemFrom(baselineItem, datas_[11].region);

    yPos = root.attribute("meanline", "null").toInt();
    QGraphicsLineItem *meanlineItem = new QGraphicsLineItem(QLine(QPoint(xPoint.x(), yPos), QPoint(xPoint.y(), yPos)));
    meanlineItem->setZValue(datas_[12].zValue);
    graphicsItem_->addItemFrom(meanlineItem, datas_[12].region);
}
