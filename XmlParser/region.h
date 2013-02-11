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
}

#endif // REGION_H
