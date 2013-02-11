#include "picture.h"

Picture::Picture(QTextEdit *textEdit)
{
    setPos(0, 0);
    isPush = false;
    painter = new QPainter(&pixmap());
    this->textEdit = textEdit;
}

Picture::Picture(const QString &filename, QTextEdit *textEdit)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setPos(0, 0);
    isPush = false;
    load(filename);
    painter = new QPainter(&pixmap());
    this->textEdit = textEdit;
}

void Picture::load(const QString &filename)
{
    QPixmap image;
    image.load(filename);
    setPixmap(image);
}

QString Picture::pressPosition()
{
    return "Mouse Press Position : " + QString::number(pressPos.x()) + ", " + QString::number(pressPos.y());
}

QString Picture::releasePosition()
{
    return "Mouse Release Position : " + QString::number(releasePos.x()) + ", " + QString::number(releasePos.y());
}

void Picture::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPush)
    {
        pressPos = event->pos();
        textEdit->setText(pressPosition());
        isPush = true;
    }
}

void Picture::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        releasePos = event->pos();
        textEdit->setText(releasePosition());
        QRectF rectangle;
        if (pressPos.x() < releasePos.x())
        {
            rectangle.setX(pressPos.x());
            rectangle.setWidth(releasePos.x() - pressPos.x());
        }
        else
        {
            rectangle.setX(releasePos.x());
            rectangle.setWidth(pressPos.x() - releasePos.x());
        }
        if (pressPos.y() < releasePos.y())
        {
            rectangle.setY(pressPos.y());
            rectangle.setHeight(releasePos.y() - pressPos.y());
        }
        else
        {
            rectangle.setY(releasePos.y());
            rectangle.setHeight(pressPos.y() - releasePos.y());
        }
        painter->drawRoundedRect(rectangle, 20.0, 15.0);

        isPush = false;
    }
}
