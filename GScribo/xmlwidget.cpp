#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QWidget(parent)
{
    viewer.setHeaderHidden(true);

    QHBoxLayout *hLayout = new QHBoxLayout;

    QStringList headerNames;
    headerNames << tr("Name") << tr("Value");

    property.setHeaderItem(new QTreeWidgetItem(headerNames));

    hLayout->addWidget(&viewer);
    hLayout->addWidget(&property);

    setLayout(hLayout);
}
