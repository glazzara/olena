#include "picture.h"

Picture::Picture()
{
    setPos(0, 0);
    isPress = false;
}

void Picture::load(const QString &filename)
{
    QPixmap image;
    image.load(filename);
    setPixmap(image);
}

void Picture::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPress)
    {
        pressPos = event->pos();
        isPress = true;
    }
}

void Picture::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        releasePos = event->pos();
        isPress = false;
    }
}
