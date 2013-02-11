#include "pageswidget.h"

PagesWidget::PagesWidget(QWidget *parent):
        QWidget(parent)
{
    filenames = new QStringList;
    scrollArea = new QScrollArea(this);
    scrollArea->setGeometry(0, 0, 1000, 1000);
    layout = new QHBoxLayout;
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    scrollArea->setWidget(widget);
}

PagesWidget::~PagesWidget()
{
    delete filenames;
    delete layout;
    delete scrollArea;
}

void PagesWidget::addPicture(const QString& filename, const QPixmap& pixmap)
{
    *filenames << filename;
    QPixmap pix = pixmap.scaled(100, 100, Qt::IgnoreAspectRatio);
    QLabel *label = new QLabel(this);
    label->setPixmap(pix);
    layout->addWidget(label, 0, Qt::AlignHCenter | Qt::AlignTop);
    scrollArea->update(scrollArea->geometry());
}
