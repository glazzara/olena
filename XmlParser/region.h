#ifndef REGION_H
#define REGION_H

#include <QColor>

namespace GraphicRegion
{
    struct Data
    {
        QColor color;
        QString name;
        int region;
        int zValue;
    };

    enum Id
    {
        Text,
        Line,
        Baseline,
        Meanline,
        HorizontalSeparator,
        VerticalSeparator,
        WhiteSpaceSeparator,
        Image,
        Noise,
        Table,
        LineDrawing,
        Graphic,
        Chart,
        Maths
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
