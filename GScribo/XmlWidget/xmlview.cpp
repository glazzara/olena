#include "xmlview.h"

XmlView::XmlView(XmlItem *rootItem)
{
    load(rootItem);
}

void XmlView::load(XmlItem *rootItem)
{
    model_->clear();
    model_->load(rootItem);
}
