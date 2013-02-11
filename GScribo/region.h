#ifndef REGION_H
#define REGION_H

#include <QColor>

namespace GraphicRegion
{
    enum Id
    {
        Text = 0,
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

    struct Data
    {
        QColor color;
        QString name;
        GraphicRegion::Id region;
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
