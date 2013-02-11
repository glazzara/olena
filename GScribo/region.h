#ifndef REGION_H
#define REGION_H

#include <QColor>

namespace GraphicsRegion
{
    enum Id
    {
        Text = 0,
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

        TextRegion,
        SeparatorRegion,
        MiscRegion,
        TypoRegion,

        None
    };

    struct Data
    {
        QColor color;
        QString name;
        GraphicsRegion::Id region;
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
