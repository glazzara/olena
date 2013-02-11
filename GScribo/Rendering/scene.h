#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QFileDialog>

#include "selection.h"
#include "polygonitem.h"

class Scene :
        public QGraphicsScene
{
        Q_OBJECT

    public:
        explicit Scene(QObject *parent = 0);
        explicit Scene(const QRectF &sceneRect, QObject *parent = 0);
        explicit Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

        QString backgroundPath() const;
        void reset();
        void addPolygonItem(QGraphicsItem *item);
        void changeScene(const QString& filename, const QPixmap& pixmap, QGraphicsItem *item = 0);
        void repaintSelection(const QRectF& rect, bool clic);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    private:
        void init();

        QGraphicsItem *item;
        Selection selection;
        QPointF pressPos;
        bool isPressing;
        QString path;
        bool clic;

    signals:
        void selectTreeItem(QTreeWidgetItem *item);
        void clearTreeItemSelection();
    };

#endif // SCENE_H
