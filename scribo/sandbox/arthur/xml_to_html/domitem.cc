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

#include <QtXml>

#include "domitem.hh"

DomItem::DomItem(QDomNode &node, int row, DomItem *parent)
{
  domNode = node;
  // Record the item's location within its parent.
  rowNumber = row;
  parentItem = parent;
}

DomItem::~DomItem()
{
  QHash<int,DomItem*>::iterator it;
  for (it = childItems.begin(); it != childItems.end(); ++it)
    delete it.value();
}

QDomNode DomItem::node() const
{
  return domNode;
}

DomItem *DomItem::parent()
{
  return parentItem;
}

DomItem *DomItem::child(int i)
{
  if (childItems.contains(i))
    return childItems[i];

  if (i >= 0 && i < domNode.childNodes().count()) {
    QDomNode childNode = domNode.childNodes().item(i);
    DomItem *childItem = new DomItem(childNode, i, this);
    childItems[i] = childItem;
    return childItem;
  }
  return 0;
}

int DomItem::row()
{
  return rowNumber;
}
