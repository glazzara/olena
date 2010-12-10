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

#ifndef SCRIBO_DEMO_VIEWER_XML_WIDGET_HH
# define SCRIBO_DEMO_VIEWER_XML_WIDGET_HH

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

#endif // ! SCRIBO_DEMO_VIEWER_XML_WIDGET_HH
