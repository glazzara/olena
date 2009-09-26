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

#ifndef   	XML_WIDGET_HH_
# define   	XML_WIDGET_HH_

# include <QtGui>

class DomModel;

class XmlWidget
  : public QWidget
{
  Q_OBJECT

public:
  XmlWidget();
  ~XmlWidget();

public slots:
  void update(DomModel* model);
  void select(const QModelIndex& index);
  void deselect(const QModelIndex& index);

private:
  QTreeView* view_;
  DomModel* model_;
};

#endif	    /* !XML_WIDGET_HH_ */
