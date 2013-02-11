#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
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

        inline QString backgroundPath() const;

        void clear();
        void addPolygonItem(QGraphicsItem *item);
        void changeScene(const QString& filename, const QPixmap& pixmap, QGraphicsItem *item = 0);
        void selectItems(const QRectF& rect, bool clic);

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
        bool clic;
        QString path;

    public slots:
        void selectItem(PolygonItem *graphicalItem);

    signals:
        void selectTreeItems(const QList<QTreeWidgetItem *>& selectionTree);
};

inline QString Scene::backgroundPath() const
{ return path; }

#endif // SCENE_H
