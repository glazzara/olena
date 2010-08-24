/****************************************************************************
 **
 ** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
 ** Contact: Qt Software Information (qt-info@nokia.com)
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License versions 2.0 or 3.0 as published by the Free
 ** Software Foundation and appearing in the file LICENSE.GPL included in
 ** the packaging of this file.  Please review the following information
 ** to ensure GNU General Public Licensing requirements will be met:
 ** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
 ** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
 ** exception, Nokia gives you certain additional rights. These rights
 ** are described in the Nokia Qt GPL Exception version 1.3, included in
 ** the file GPL_EXCEPTION.txt in this package.
 **
 ** Qt for Windows(R) Licensees
 ** As a special exception, Nokia, as the sole copyright holder for Qt
 ** Designer, grants users of the Qt/Eclipse Integration plug-in the
 ** right for the Qt/Eclipse Integration to link to functionality
 ** provided by Qt Designer and its related libraries.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at qt-sales@nokia.com.
 **
 ****************************************************************************/

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

#include <QtGui>
#include <QtXml>

#include "domitem.hh"
#include "dommodel.hh"

DomModel::DomModel(QDomDocument document, QObject *parent)
  : QAbstractItemModel(parent), domDocument(document)
{
  rootItem = new DomItem(domDocument, 0);
}

DomModel::~DomModel()
{
  delete rootItem;
}

int DomModel::columnCount(const QModelIndex &/*parent*/) const
{
  return 3;
}

QVariant DomModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  DomItem *item = static_cast<DomItem*>(index.internalPointer());
  QDomNode node = item->node();
  QDomNamedNodeMap attributeMap = node.attributes();

  if (role == Qt::DisplayRole)
  {
    QStringList attributes;

    switch (index.column())
    {
    case 0:
      return node.nodeName();
    case 1:
      for (int i = 0; i < attributeMap.count(); ++i)
      {
	QDomNode attribute = attributeMap.item(i);
	attributes << attribute.nodeName() + "=\""
	  + attribute.nodeValue() + "\"";
      }
      return attributes.join(" ");
    case 2:
      return node.nodeValue().split("\n").join(" ");
    default:
      return QVariant();
    }
  }
  else if (role == Qt::UserRole)
  {
    QMap<QString, QVariant> attributes;
    switch (index.column())
    {
    case 1:
      for (int i = 0; i < attributeMap.count(); ++i)
      {
	QDomNode attribute = attributeMap.item(i);
	attributes[attribute.nodeName()] = attribute.nodeValue();
      }
      return attributes;
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DomModel::headerData(int section, Qt::Orientation orientation,
			      int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
    case 0:
      return tr("Name");
    case 1:
      return tr("Attributes");
    case 2:
      return tr("Value");
    default:
      return QVariant();
    }
  }

  return QVariant();
}

QModelIndex DomModel::index(int row, int column, const QModelIndex &parent)
  const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  DomItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<DomItem*>(parent.internalPointer());

  DomItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex DomModel::parent(const QModelIndex &child) const
{
  if (!child.isValid())
    return QModelIndex();

  DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
  DomItem *parentItem = childItem->parent();

  if (!parentItem || parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int DomModel::rowCount(const QModelIndex &parent) const
{
  if (parent.column() > 0)
    return 0;

  DomItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<DomItem*>(parent.internalPointer());

  return parentItem->node().childNodes().count();
}
