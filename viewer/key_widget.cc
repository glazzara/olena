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

#include "key_widget.hh"
#include "common.hh"

KeyWidget::KeyWidget(const region::KeyMap& key_map)
  : items_(new QListWidget())
{
  QLabel* title = new QLabel(tr("Key"));
  title->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(items_);

  for (int i = 0; i < key_map.size(); ++i)
    add_item_(key_map[i].first, key_map[i].second);

  setLayout(layout);

  connect(items_, SIGNAL(itemChanged(QListWidgetItem*)),
	  this, SLOT(update(QListWidgetItem*)));
}

void
KeyWidget::add_item_(QString text, QColor color)
{
  QListWidgetItem* item = new QListWidgetItem(text);
  QPixmap pixmap(10, 6);
  pixmap.fill(color);
  item->setIcon(QIcon(pixmap));
  item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  item->setCheckState(Qt::Checked);
  items_->addItem(item);
}

bool
KeyWidget::isChecked(region::RegionId id)
{
  return items_->item(id)->checkState() == Qt::Checked;
}

void
KeyWidget::update(QListWidgetItem* item)
{
  int id = items_->row(item);

  emit updated(id, item->checkState() == Qt::Checked);
}

KeyWidget::~KeyWidget()
{
}
