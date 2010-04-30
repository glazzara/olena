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

# include <QtGui/QtGui>

namespace mln
{

  namespace demo
  {

    class custom_delegate : public QItemDelegate
    {

      typedef QItemDelegate super_;

    public:

      custom_delegate(QObject * parent = 0);
      ~custom_delegate();

      void
      drawDecoration(QPainter *painter,
		     const QStyleOptionViewItem& option,
		     const QRect& rect,
		     const QPixmap& pixmap) const;

      void drawBackground(QPainter *painter,
			  const QStyleOptionViewItem& option,
			  const QModelIndex& index) const;

      void drawCheck(QPainter *painter,
		     const QStyleOptionViewItem& option,
		     const QRect& rect,
		     Qt::CheckState state) const;

      void
      drawDisplay(QPainter* painter,
		  const QStyleOptionViewItem& option,
		  const QRect& rect,
		  const QString& text) const;

      void
      drawFocus(QPainter* painter,
		const QStyleOptionViewItem& option,
		const QRect& rect ) const;

    private:
      mutable QRect display_rect_;
      mutable QRect decoration_rect_;
      QBrush bg_;
      QBrush selected_bg_;

    };


  } // end of namespace mln::demo

} // end of namespace mln
