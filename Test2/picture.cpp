#include "picture.h"

Picture::Picture()
{
}

void Picture::load(const QString &filename)
{
    QPixmap image;
    image.load(filename);
    setPixmap(image);
}
