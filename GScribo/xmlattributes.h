#ifndef XMLATTRIBUTES_H
#define XMLATTRIBUTES_H

#include <QDomNamedNodeMap>

class XmlAttributes
{
    public:
        explicit XmlAttributes(XmlAttributes *precItem = 0);
        explicit XmlAttributes(const QDomNamedNodeMap& node);
        explicit XmlAttributes(const QDomNamedNodeMap& node, XmlAttributes *precItem = 0);
        ~XmlAttributes();

        inline QString name() const;
        inline QString value() const;
        inline int row();

    private:
        void init(XmlAttributes *precItem = 0);

        QString n;
        QString v;
        int r;
};

#endif // XMLATTRIBUTES_H
