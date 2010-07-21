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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

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
	  else
	    {
	      QTreeWidgetItem* child_son =
		new QTreeWidgetItem(QStringList("data = base64-encoded"));

	      child->addChild(child_son);
	    }
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
