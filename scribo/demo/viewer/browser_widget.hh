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

#ifndef SCRIBO_DEMO_VIEWER_BROWSER_WIDGET_HH
# define SCRIBO_DEMO_VIEWER_BROWSER_WIDGET_HH

# include <QtGui>

class BrowserWidget
  : public QWidget
{
  Q_OBJECT

public:
  BrowserWidget(QDirModel* files, QString dir = QString());
  ~BrowserWidget();

public slots:
  void activate(const QModelIndex& index, bool b = false);
  void path_return_pressed();
  void next() { change_pos(true); }
  void prev() { change_pos(false); }

signals:
  void activated(QString filename, bool b, bool x);

private:
  void change_pos(bool next);
  QDirModel* files_;
  QListView* view_;
  QLineEdit* path_;
  bool first_time_;
};

#endif // ! SCRIBO_DEMO_VIEWER_BROWSER_WIDGET_HH
