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

#include "property_widget.hh"

#include "dommodel.hh"

PropertyWidget::PropertyWidget()
  : view_(new QTreeWidget()),
    model_(0)
{
  QLabel* title = new QLabel(tr("Properties"));
  title->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(view_);

  view_->setColumnCount(2);
  view_->setSortingEnabled(true);
  view_->setRootIsDecorated(false);
  QStringList header_names;
  header_names << tr("Name") << tr("Value");
  view_->setHeaderItem(new QTreeWidgetItem(header_names));

  setLayout(layout);
}

void
PropertyWidget::update(DomModel* model)
{
  model_ = model;
}

void
PropertyWidget::select(const QModelIndex& index)
{
  if (!index.isValid())
    return;

  QMap<QString, QVariant> data =
    model_->data(index, Qt::UserRole).toMap();

  for (QMap<QString, QVariant>::iterator i = data.begin();
       i != data.end();
       ++i)
  {
    QStringList values;
    values << i.key() << i.value().toString();
    view_->addTopLevelItem(new QTreeWidgetItem(values));
  }
}

void
PropertyWidget::deselect(const QModelIndex&)
{
  view_->clear();
}

PropertyWidget::~PropertyWidget()
{
}
