#include "pageswidget.h"

PagesWidget::PagesWidget(QWidget *parent):
        QWidget(parent)
{
    QStringList l;
    l << "/lrde/home/stage/froger_a/ppm/mp00032c.ppm" << "/lrde/home/stage/froger_a/ppm/mp00042.ppm" << "/lrde/home/stage/froger_a/ppm/mp00076.ppm";
    filenames = new QStringList;
    model = new ListModel(l);
    listView = new QListView(this);
    listView->setItemDelegate(new ItemDelegate());
    listView->setModel(model);
    listView->update(model->index(0, 0));
    listView->show();
}

PagesWidget::~PagesWidget()
{
    delete filenames;
    delete listView;
}

void PagesWidget::addPicture(const QString& filename, const QPixmap& pixmap)
{
    *filenames << filename;

}
