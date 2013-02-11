#include "xml.h"

Xml::Xml()
{
}

Xml::Xml(const QString &filename)
{
    load(filename);
}

void Xml::load(const QString &filename)
{
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);

    QDomDocument xml;
    xml.setContent(&xmlFile);

    xmlFile.close();

    QDomElement root = xml.documentElement();
    tItems.setText(0, QStringList(root.tagName()));
}
