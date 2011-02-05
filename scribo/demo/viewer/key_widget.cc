// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

#include "key_widget.hh"
#include "common.hh"

KeyWidget::KeyWidget(const region::KeyMap& key_map)
  : items_(new QTreeWidget())
{
  item_list_.append(new QTreeWidgetItem(QStringList("Text")));
  item_list_.append(new QTreeWidgetItem(QStringList("Separators")));
  item_list_.append(new QTreeWidgetItem(QStringList("Misc. Regions")));
  item_list_.append(new QTreeWidgetItem(QStringList("Typographical lines")));

  QLabel* title = new QLabel(tr("Key"));
  title->setAlignment(Qt::AlignHCenter);

  foreach(QTreeWidgetItem* item, item_list_)
  {
    items_->addTopLevelItem(item);
    item->setCheckState(0, Qt::Checked);
    item->setExpanded(true);
  }
  items_->setHeaderHidden(true);

  base_id_.append(0);
  base_id_.append(region::EndOfTextRegion + 1);
  base_id_.append(region::EndOfSepsRegion + 1);
  base_id_.append(region::EndOfMiscRegion + 1);
  base_id_.append(region::EndOfTypoRegion + 1);

  for (int j = 0; j < base_id_.size() - 1; ++j)
    for (int i = base_id_.at(j); i < base_id_.at(j + 1) - 1; ++i)
      add_item_(key_map.at(i).first, key_map.at(i).second, false, item_list_.at(j));

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(items_);

  setLayout(layout);

  connect(items_, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
	    this, SLOT(update(QTreeWidgetItem*)));
}

void KeyWidget::update_all()
{
  foreach(QTreeWidgetItem* item, item_list_)
    for (int i = 0; i < item->childCount(); ++i)
      update(item->child(i));
}

void KeyWidget::setAll(bool b)
{
  Qt::CheckState state;
  if (b)
    state = Qt::Checked;
  else
    state = Qt::Unchecked;

  foreach(QTreeWidgetItem* item, item_list_)
    item->setCheckState(0, state);
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
  item_list_.at(0)->child(region::Line)->setHidden(!b);
  // FIXME: we may like to hide also typographical objects.
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
  QTreeWidgetItem* current_item = 0;

  foreach(QTreeWidgetItem* item, item_list_)
  {
    current_item = item->child(id);
    if (item)
      break;
    else
      current_item = 0;
  }

  if (!current_item)
    return false;

  return current_item->checkState(0) == Qt::Checked;
}

void
KeyWidget::update(QTreeWidgetItem* item_up)
{
  int id = -1;

  foreach(QTreeWidgetItem* item, item_list_)
    if (item_up == item)
    {
      setAllCheck(item);
      return;
    }


  int i = 0;
  foreach(QTreeWidgetItem* item, item_list_)
  {
    id = item->indexOfChild(item_up);
    if (id != -1)
    {
      id += base_id_.at(i);
      break;
    }
    ++i;
  }

  emit updated(id, item_up->checkState(0) == Qt::Checked);
}

KeyWidget::~KeyWidget()
{
}

