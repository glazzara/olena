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

#ifndef   	BROWSER_WIDGET_HH_
# define   	BROWSER_WIDGET_HH_

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

#endif	    /* !BROWSER_WIDGET_HH_ */
