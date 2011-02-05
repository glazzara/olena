// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_DEMO_VIEWER_KEY_WIDGET_HH
# define SCRIBO_DEMO_VIEWER_KEY_WIDGET_HH

# include <QtGui>
# include "common.hh"

class KeyWidget
  : public QWidget
{
  Q_OBJECT

public:
  KeyWidget(const region::KeyMap& key_map);
  ~KeyWidget();

  bool isChecked(region::RegionId id);
  QTreeWidget* items() { return items_; };
  void update_all();

signals:
  void updated(int key, bool checked);

public slots:
  void setAll(bool b);

private slots:
  void change_mode(bool b);
  void update(QTreeWidgetItem* item);
  void setAllCheck(QTreeWidgetItem* parent);

private:
  void add_item_(QString text, QColor color, bool b, QTreeWidgetItem* parent);

  QTreeWidget* items_;
  QVector<QTreeWidgetItem*> item_list_;
  QVector<int> base_id_;
};

#endif // ! SCRIBO_DEMO_VIEWER_KEY_WIDGET_HH
