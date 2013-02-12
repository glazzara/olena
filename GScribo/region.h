#ifndef REGION_H
#define REGION_H

#include <QColor>

namespace GraphicsRegion
{
    // WARNING : The order is important.
    enum Id
    {
        None,

        Text,
        TextRegion,
        Line,

        Separators,
        VerticalSeparator,
        HorizontalSeparator,
        WhiteSpaceSeparator,

        Miscellaneous,
        Image,
        Noise,
        Table,
        Maths,
        Graphic,
        Chart,

        Typology,
        Baseline,
        Meanline
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
