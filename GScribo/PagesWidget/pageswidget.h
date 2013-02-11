#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include <QListView>
#include <QShortcut>

#include "pagesdelegate.h"
#include "listmodel.h"

class PagesWidget:
        public QListView
{
        Q_OBJECT

    public:
        explicit PagesWidget(QWidget *parent = 0);

        inline QStringList filenames() const;
        inline int currentRow() const;
        inline void setCurrentRow(int currentRow);

        inline void addPicture(const QString& filename, const QPixmap& pixmap);

    private:
        ListModel model_;

    public slots:
        void removeSelection();

    private slots:
        inline void getPicture(const QModelIndex& index);

    signals:
        void imageSelectionned(const QString& filename);
};

inline QStringList PagesWidget::filenames() const
{ return model_.filenames(); }

inline int PagesWidget::currentRow() const
{ return model_.currentRow(); }

inline void PagesWidget::setCurrentRow(int currentRow)
{ model_.setCurrentRow(currentRow); }

void PagesWidget::getPicture(const QModelIndex& index)
{ QString filename = index.data(Qt::UserRole).toString(); emit imageSelectionned(filename); }

inline void PagesWidget::addPicture(const QString& filename, const QPixmap& pixmap)
{ model_.addPicture(filename, pixmap); }

#endif // PAGESWIDGET_H
