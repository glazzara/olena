// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <src/custom_delegate.hh>

#include <QtCore/QtCore>

namespace mln
{

  namespace demo
  {

    custom_delegate::custom_delegate(QObject * parent)
      : QItemDelegate(parent),
	bg_(Qt::black, Qt::SolidPattern),
	selected_bg_(QColor(255, 100, 0), Qt::Dense5Pattern)
    {
      display_rect_.setSize(QSize(149, 30));
      decoration_rect_.setSize(QSize(150, 150));
    }


    custom_delegate::~custom_delegate()
    {
    }

    void
    custom_delegate::drawDecoration(QPainter *painter,
				    const QStyleOptionViewItem& option,
				    const QRect& rect,
				    const QPixmap& pixmap) const
    {
      decoration_rect_.moveTo(option.rect.x() + 10, option.rect.y() + 10);
      painter->fillRect(decoration_rect_, bg_);
      super_::drawDecoration(painter, option, decoration_rect_, pixmap);
    }


    void
    custom_delegate::drawBackground(QPainter *painter,
				    const QStyleOptionViewItem& option,
				    const QModelIndex& index) const
    {
      super_::drawBackground(painter, option, index);
    }


    void
    custom_delegate::drawCheck(QPainter *painter,
			       const QStyleOptionViewItem& option,
			       const QRect& rect,
			       Qt::CheckState state) const
    {
      super_::drawCheck(painter, option, rect, state);
    }


    void
    custom_delegate::drawDisplay(QPainter* painter,
				 const QStyleOptionViewItem& option,
				 const QRect& rect,
				 const QString& text) const
    {
      display_rect_.moveTo(option.rect.x() + 10,
			   option.rect.y() + 155);
      super_::drawDisplay(painter, option, display_rect_, text);
      painter->drawRect(display_rect_);
    }


    void
    custom_delegate::drawFocus(QPainter* painter,
			       const QStyleOptionViewItem& option,
			       const QRect& rect ) const
    {
      super_::drawFocus(painter, option, display_rect_);
    }


  } // end of namespace mln::demo

} // end of namespace mln
