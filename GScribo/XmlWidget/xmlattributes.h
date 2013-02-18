#ifndef XMLATTRIBUTES_H
#define XMLATTRIBUTES_H

#include <QDomNamedNodeMap>
#include <QStringList>
#include <QVariant>

class XmlAttributes
{
    public:
        explicit XmlAttributes();
        explicit XmlAttributes(const QDomNamedNodeMap& node);

        void load(const QDomNamedNodeMap& node);

        inline QStringList names() const;
        inline QStringList values() const;

    private:
        QStringList names_;
        QStringList values_;
};

inline QStringList XmlAttributes::names() const
{ return names_; }

inline QStringList XmlAttributes::values() const
{ return values_; }

Q_DECLARE_METATYPE(XmlAttributes)
Q_DECLARE_METATYPE(XmlAttributes *)

#endif // XMLATTRIBUTES_H
