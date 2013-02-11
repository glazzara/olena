#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>

#include "rootgraphicsitem.h"
#include "polygonitem.h"
#include "selection.h"

class Scene :
        public QGraphicsScene
{
        Q_OBJECT

    public:
        explicit Scene(QObject *parent = 0);
        explicit Scene(const QRectF &sceneRect, QObject *parent = 0);
        explicit Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

        inline QString backgroundPath() const;

        void setRootItem(RootGraphicsItem *graphicalItem);
        inline RootGraphicsItem *rootItem() const;

        inline QList<QGraphicsItem *> selectedItems() const;
        void selectItems(const QRectF& rect);
        void selectItems(const QPointF& point);

        void changeScene(const QString& filename, RootGraphicsItem *rootItem = 0);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    private:
        void init();
        void selectItems(QGraphicsItem *root, const QPointF& point, const QRectF& rect);

        QString backgroundPath_;
        RootGraphicsItem *rootItem_;

        Selection selection_;
        QList<QGraphicsItem *> selectedItems_;

        QPointF pressPos_;
        bool isPressing_;

    public slots:
        void clear();
        void clearSelection();
        void selectItems(const QList<PolygonItem *>& selectedItems, bool addToSelection);

    signals:
        void beginSelection();
        void endSelection(const QList<QGraphicsItem *>& selectedItems);
};

inline QString Scene::backgroundPath() const
{ return backgroundPath_; }

inline RootGraphicsItem *Scene::rootItem() const
{ return rootItem_; }

inline QList<QGraphicsItem *> Scene::selectedItems() const
{ return selectedItems_; }

inline void Scene::selectItems(const QPointF& point)
{ selectItems(rootItem_, point, QRectF()); }

inline void Scene::selectItems(const QRectF& rect)
{ selectItems(rootItem_, QPointF(), rect); }

#endif // SCENE_H
