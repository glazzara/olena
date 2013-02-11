#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>

class PagesWidget:
        public QWidget
{
        Q_OBJECT

    public:
        explicit PagesWidget(QWidget *parent = 0);
        explicit PagesWidget(QWidgetPrivate &d, QWidget *parent = 0);
        ~PagesWidget();
        void addPicture(const QString& filename, const QPixmap& pixmap);

    private:
        QScrollArea *scrollArea;
        QHBoxLayout *layout;
        QStringList *filenames;
};

#endif // PAGESWIDGET_H
