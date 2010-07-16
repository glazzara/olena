//
// Document layout viewer.
//
// Copyright (C) 2009 Florent D'Halluin.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include "xml_widget.hh"

#include "dommodel.hh"

XmlWidget::XmlWidget()
  : view_(new QTreeView()),
    model_(0)
{
  QLabel* title = new QLabel(tr("XML"));
  title->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* layout = new QVBoxLayout;

  layout->addWidget(title);
  layout->addWidget(view_);

  setLayout(layout);
}

void
XmlWidget::update(DomModel* model)
{
  view_->setModel(model);
  //  view_->resizeColumnToContents(2);
}

void
XmlWidget::select(const QModelIndex& index)
{
  view_->setCurrentIndex(index);
}

void
XmlWidget::deselect(const QModelIndex&)
{
}

XmlWidget::~XmlWidget()
{
}
