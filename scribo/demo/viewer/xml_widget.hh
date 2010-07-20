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
# include <QDomNode>

class DomModel;

class XmlWidget
  : public QWidget
{
  Q_OBJECT

public:
  XmlWidget();
  ~XmlWidget();

public slots:
  void select(QString id, QString name);
  void deselect();
  void fill_widget(QString xml);
  void check_item (QTreeWidgetItem* item);

private:
  void NFS(QDomNode node, QTreeWidgetItem* item);
  QTreeWidget* tree_;
  QTreeWidget* property_;
  QMap<QString, QTreeWidgetItem*> item_map_;
  QMap<QTreeWidgetItem*, QDomNode> node_map_;
};

#endif	    /* !XML_WIDGET_HH_ */
