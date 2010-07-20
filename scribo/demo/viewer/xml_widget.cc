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

#include "xml_widget.hh"

XmlWidget::XmlWidget()
  : tree_(new QTreeWidget()),
    property_(new QTreeWidget())
{
  QLabel* title = new QLabel(tr("XML"));
  title->setAlignment(Qt::AlignHCenter);

  tree_->setHeaderHidden(true);

  QVBoxLayout* layout = new QVBoxLayout;
  QHBoxLayout *hlayout = new QHBoxLayout;

  QStringList header_names;
  header_names << tr("Name") << tr("Value");
  property_->setHeaderItem(new QTreeWidgetItem(header_names));

  hlayout->addWidget(tree_);
  hlayout->addWidget(property_);

  layout->addWidget(title);
  layout->addLayout(hlayout);

  setLayout(layout);

  connect(tree_, SIGNAL(itemDoubleClicked (QTreeWidgetItem*, int)),
	  this, SLOT(check_item (QTreeWidgetItem*) ) );
}

void
XmlWidget::select(QString id, QString name)
{
  QDomNode n = node_map_[item_map_[id]];

  if (name.contains("Paragraph"))
    {
      n = n.firstChild();
      while (!n.isNull() && !n.toElement().tagName().contains("paragraph"))
	  n = n.nextSibling();
    }

 if (name.contains("Text line"))
    {
      n = n.firstChild();
      while (!n.isNull() && !n.toElement().tagName().contains("paragraph"))
	  n = n.nextSibling();

      if (!n.isNull())
	{
	  n = n.firstChild();
	  while (!n.isNull() && !n.toElement().tagName().contains("line"))
	    n = n.nextSibling();
	}
    }

  if (!n.isNull())
    {
      QTreeWidgetItem* item = node_map_.key(n);

      tree_->setCurrentItem(item, 0);
      item->setExpanded(true);
      check_item(item);
    }
}

void
XmlWidget::check_item (QTreeWidgetItem* item)
{
  QDomNode node = node_map_[item];

  if (node.hasAttributes())
    {
      property_->clear();
      QDomNamedNodeMap attributes = node.toElement().attributes();

      for (int i = 0; i < attributes.count(); ++i)
	{
	  QStringList values;
	  values << attributes.item(i).toAttr().name() << attributes.item(i).toAttr().value();
	  property_->addTopLevelItem(new QTreeWidgetItem(values));
	}
    }

  property_->resizeColumnToContents(0);
}

void XmlWidget::NFS(QDomNode node, QTreeWidgetItem* item)
{
  if (!node.isNull())
    {
      QString append;
      if (node.toElement().tagName().contains("point"))
	{
	  QString x = node.toElement().attribute("x", "0");
	  QString y = node.toElement().attribute("y", "0");
	  append.append(" = (" + x + ", " + y + ")");
	}

      QTreeWidgetItem* child =
	new QTreeWidgetItem(QStringList(node.toElement().tagName() + node.nodeValue() + append));

      if (node.hasAttributes())
	{
	  QString id = node.toElement().attribute("id", "none");
	  item_map_[id] = child;
	}

      node_map_[child] = node;
      item->addChild(child);

      QDomNode sibling = node.firstChild();
      while (!sibling.isNull())
	{
	  if (!sibling.toElement().tagName().contains("data"))
	    NFS(sibling, child);
	  sibling = sibling.nextSibling();
	}
    }
}

void XmlWidget::fill_widget(QString xml)
{
  node_map_.clear();
  item_map_.clear();

  tree_->clear();
  property_->clear();
  QFile f_in(xml);
  f_in.open(QIODevice::ReadOnly);

  QDomDocument doc;
  doc.setContent(&f_in);
  f_in.close();

  int i = 0;

  QDomElement root = doc.documentElement();
  QTreeWidgetItem* root_item=
	new QTreeWidgetItem(QStringList(root.tagName()));

  tree_->addTopLevelItem(root_item);
  root = root.firstChild().toElement();

  while (!root.isNull())
    {
      ++i;
      NFS(root, root_item);
      root = root.nextSibling().toElement();
    }
}

void
XmlWidget::deselect()
{
  property_->clear();
}

XmlWidget::~XmlWidget()
{
}
