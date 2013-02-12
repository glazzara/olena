#ifndef REGION_H
#define REGION_H

#include <QColor>

namespace GraphicsRegion
{
    // WARNING : The order is important.
    enum Id
    {
        TextRegion,
        Line,

        VerticalSeparator,
        HorizontalSeparator,
        WhiteSpaceSeparator,

        Image,
        Noise,
        Table,
        Maths,
        Graphic,
        Chart,

        Baseline,
        Meanline,

        Text,
        Separators,
        Miscellaneous,
        Typology,

        None
    };

    struct Data
    {
        QColor color;
        QString name;
        GraphicsRegion::Id region;
        GraphicsRegion::Id parent;
        int zValue;
    };
}

namespace Separator
{
    enum FindSeparator
    {
        Lines,
        Whitespaces,
        Both
    };
}

#endif // REGION_H
