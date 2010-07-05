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
    add_item_(key_map[i].first, key_map[i].second, 
	      (i == region::Paragraph) || 
	      (i == region::Line) );

  setLayout(layout);

  connect(items_, SIGNAL(itemChanged(QListWidgetItem*)),
	  this, SLOT(update(QListWidgetItem*)));
}

void KeyWidget::checkAll()
{
  for (int i = 0; i < items_->count(); ++i)
    {
      items_->item(i)->setCheckState(Qt::Checked);
      emit updated(i, true);      
    }
}

void 
KeyWidget::change_mode(bool b)
{
  int id_region = region::Paragraph;
  int id_line = region::Line;
  if (b)
    {
      items_->item(id_region)->setCheckState(Qt::Checked);
      items_->item(id_region)->setHidden(false);
      items_->item(id_line)->setCheckState(Qt::Checked);
      items_->item(id_line)->setHidden(false);
    }
  else
    {
      items_->item(id_region)->setCheckState(Qt::Unchecked);
      items_->item(id_region)->setHidden(true);
      items_->item(id_line)->setCheckState(Qt::Unchecked);
      items_->item(id_line)->setHidden(true);
    }
}

void
KeyWidget::add_item_(QString text, QColor color, bool b)
{
  QListWidgetItem* item = new QListWidgetItem(text);
  QPixmap pixmap(10, 6);
  pixmap.fill(color);
  item->setIcon(QIcon(pixmap));
  item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  item->setCheckState(Qt::Checked);
  items_->addItem(item);
  int id = items_->row(item);
  items_->item(id)->setHidden(b);
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
