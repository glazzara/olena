#ifndef PAGESMODEL_H
#define PAGESMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include <QList>

class PagesModel:
        public QAbstractListModel
{
        Q_OBJECT

    public:
        explicit PagesModel(QObject *parent = 0);

        inline QStringList filenames() const;
        inline int rowCount(const QModelIndex& parent = QModelIndex()) const;

        inline int currentRow() const;
        void setCurrentRow(int currentRow);

        QVariant data(const QModelIndex& index, int role = Qt::DecorationRole) const;
        void addPicture(const QString& filename, const QPixmap& pixmap);

    private:
        QStringList filenames_;
        QList<QPixmap> pixmaps_;
        int currentRow_;

    public slots:
        void removePixmap(const QModelIndex& parent);
};

inline QStringList PagesModel::filenames() const
{ return filenames_; }

inline int PagesModel::rowCount(const QModelIndex&) const
{ return filenames_.count(); }

inline int PagesModel::currentRow() const
{ return currentRow_; }

#endif // PAGESMODEL_H
