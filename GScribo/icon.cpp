#include "icon.h"

Icon::Icon(const QPixmap &pixmap) :
        QIcon(pixmap)
{
}

QString Icon::path() const
{
    return filename;
}

QPixmap * Icon::pixmap()
{
    return pix;
}

void Icon::setPath(QString path)
{
    filename = path;
}

void Icon::setPixmap(QPixmap *pixmap)
{
    pix = pixmap;
}
