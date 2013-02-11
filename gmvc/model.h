#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <QIcon>

class Model :
        public QAbstractListModel
{
    public:
        explicit Model(QObject *parent = 0);
        QVariant data(const QModelIndex &index, int role) const;
        int rowCount(const QModelIndex &parent) const;
        void addPixmap(const QPixmap& pixmap);

    private:
        QList<QPixmap> pixmaps;
};

#endif // MODEL_H
