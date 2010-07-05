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

#ifndef   	KEY_WIDGET_HH_
# define   	KEY_WIDGET_HH_

# include <QtGui>
# include "common.hh"

class KeyWidget
  : public QWidget
{
  Q_OBJECT

public:
  KeyWidget(const region::KeyMap& key_map);
  ~KeyWidget();

  void checkAll();
  bool isChecked(region::RegionId id);

signals:
  void updated(int key, bool checked);

private slots:
  void update(QListWidgetItem* slot);
  void change_mode(bool b);

private:
  void add_item_(QString text, QColor color, bool b);

  QListWidget* items_;
};

#endif	    /* !KEY_WIDGET_HH_ */
