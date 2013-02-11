#include "picture.h"
#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QTextEdit *textEdit)
{
    this->textEdit = textEdit;
    color.setRgb(100, 200, 60, 100);
    isPress = false;
}

QString CustomGraphicsView::mousePressPrinter()
{
    return "Mouse Press Position On View : " + QString::number(pressPos.x()) + ", " + QString::number(pressPos.y());
}

QString CustomGraphicsView::mouseReleasePrinter()
{
    return "Mouse Press Position On View : " + QString::number(pressPos.x()) + ", " + QString ::number(pressPos.y()) +
           "\nMouse Release Position On View : " + QString::number(releasePos.x()) + ", " + QString::number(releasePos.y());
}

QString CustomGraphicsView::mouseMovePrinter(qreal x, qreal y)
{
    return "Mouse Position On View : "  + QString::number(x) + ", " + QString::number(y);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !isPress)
    {
        pressPos = event->posF();
        rectangle.setX(pressPos.x());
        rectangle.setY(pressPos.y());
        isPress = true;
        textEdit->setText(mousePressPrinter());
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        releasePos = event->posF();
        rectangle.setWidth(event->posF().x() - rectangle.x());
        rectangle.setHeight(event->posF().y() - rectangle.y());
        isPress = false;
        textEdit->setText(mouseReleasePrinter());
        viewport()->repaint();
    }
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(isPress)
    {
        rectangle.setWidth(event->posF().x() - rectangle.x());
        rectangle.setHeight(event->posF().y() - rectangle.y());
        textEdit->setText(mouseMovePrinter(event->posF().x(), event->posF().y()));
        viewport()->repaint();
    }
}

void CustomGraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    /*QPainter *painter = new QPainter(viewport());
    painter->drawRect(rectangle);
    painter->fillRect(rectangle, color);
    delete painter;*/
}
