#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout;

    viewer_.setModel(new XmlModel());
    viewer_.setItemDelegate(new TreeDelegate());

    hLayout->addWidget(&viewer_);
    hLayout->addWidget(&selection_);
    hLayout->addWidget(&property_);

    setLayout(hLayout);
}

void XmlWidget::changeView(XmlItem *rootItem)
{
    viewer_.load(rootItem);
}
