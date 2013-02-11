#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTextEdit>
#include <QMouseEvent>
#include <QRectF>
#include <QPainter>

class CustomGraphicsView :
        public QGraphicsView
{
    public:
        CustomGraphicsView(QTextEdit *textEdit);

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void paintEvent(QPaintEvent *event);

    private:
        QString mousePressPrinter();
        QString mouseReleasePrinter();
        QString mouseMovePrinter(qreal x, qreal y);

        QTextEdit *textEdit;
        QPointF pressPos;
        QPointF releasePos;
        QRectF rectangle;
        QColor color;
        bool isPress;
};

#endif // CUSTOMGRAPHICSVIEW_H
