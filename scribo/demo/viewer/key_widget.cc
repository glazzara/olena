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
  : items_(new QTreeWidget()),
    text_(new QTreeWidgetItem(QStringList("Text"))),
    regions_(new QTreeWidgetItem(QStringList("Regions")))
{
  QLabel* title = new QLabel(tr("Key"));
  title->setAlignment(Qt::AlignHCenter);

  items_->addTopLevelItem(text_);
  items_->addTopLevelItem(regions_);
  items_->setHeaderHidden(true);

  text_->setCheckState(0, Qt::Checked);
  regions_->setCheckState(0, Qt::Checked);
  regions_->setExpanded(true);
  text_->setExpanded(true);

  for (int i = 0; i < 3; ++i)
    add_item_(key_map.at(i).first, key_map.at(i).second,
	      i == region::Line ||
	      i == region::Paragraph, text_);

  for (int i = 3; i < key_map.size(); ++i)
    add_item_(key_map.at(i).first, key_map.at(i).second, false, regions_);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(items_);

  setLayout(layout);

  connect(items_, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
	    this, SLOT(update(QTreeWidgetItem*)));
}

void KeyWidget::update_all()
{
  for (int i = 0; i < text_->childCount(); ++i)
      update(text_->child(i));

  for (int i = 0; i < regions_->childCount(); ++i)
      update(regions_->child(i));
}

void KeyWidget::setAllCheck(QTreeWidgetItem* parent)
{
  for (int i = 0; i < parent->childCount(); ++i)
    {
      if (parent->checkState(0) == Qt::Checked)
	parent->child(i)->setCheckState(0, Qt::Checked);
      else
	parent->child(i)->setCheckState(0, Qt::Unchecked);
      update(parent->child(i));
    }
}

void
KeyWidget::change_mode(bool b)
{
  int id_region = region::Paragraph;
  int id_line = region::Line;

  if (b)
    {
      text_->child(id_region)->setCheckState(0, Qt::Checked);
      text_->child(id_region)->setHidden(false);
      text_->child(id_line)->setCheckState(0, Qt::Checked);
      text_->child(id_line)->setHidden(false);
    }
  else
    {
      text_->child(id_region)->setCheckState(0, Qt::Unchecked);
      text_->child(id_region)->setHidden(true);
      text_->child(id_line)->setCheckState(0, Qt::Unchecked);
      text_->child(id_line)->setHidden(true);
    }
}

void
KeyWidget::add_item_(QString text, QColor color, bool b, QTreeWidgetItem* parent)
{
  QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(text));
  QPixmap pixmap(10, 6);

  pixmap.fill(color);
  item->setIcon(0, QIcon(pixmap));
  item->setCheckState(0, Qt::Checked);
  parent->addChild(item);
  item->setHidden(b);
}

  bool
KeyWidget::isChecked(region::RegionId id)
{
  QTreeWidgetItem* item = text_->child(id);
  if (item)
    return item->checkState(0) == Qt::Checked;
  else
    {
      item = regions_->child(id);
      if(item)
	return item->checkState(0) == Qt::Checked;
      else
	return false;
    }
}

void
KeyWidget::update(QTreeWidgetItem* item)
{
  int id;
  id = text_->indexOfChild(item);
  if (id == -1)
    id = regions_->indexOfChild(item) + 3;

  emit updated(id, item->checkState(0) == Qt::Checked);

  if (item == text_ || item == regions_)
    {
      setAllCheck(item);
      return;
    }
}

KeyWidget::~KeyWidget()
{
}
