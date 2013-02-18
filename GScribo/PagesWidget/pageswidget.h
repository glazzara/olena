#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include <QListView>
#include <QShortcut>

#include "pagesdelegate.h"
#include "pagesmodel.h"

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

    protected:
        inline void focusOutEvent(QFocusEvent *event);

    private:
        PagesModel model_;

    public slots:
        void removeSelection();

    private slots:
        void getPicture(const QModelIndex& index);

    signals:
        void imageSelectionned(const QString& filename);
};

inline QStringList PagesWidget::filenames() const
{ return model_.filenames(); }

inline int PagesWidget::currentRow() const
{ return model_.currentRow(); }

inline void PagesWidget::setCurrentRow(int currentRow)
{ model_.setCurrentRow(currentRow); }

inline void PagesWidget::addPicture(const QString& filename, const QPixmap& pixmap)
{ model_.addPicture(filename, pixmap); }

inline void PagesWidget::focusOutEvent(QFocusEvent *event)
{ QListView::focusOutEvent(event); clearSelection(); }

#endif // PAGESWIDGET_H
