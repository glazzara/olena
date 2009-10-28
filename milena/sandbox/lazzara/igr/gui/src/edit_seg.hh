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

#ifndef MLN_DEMO_SRC_EDIT_SEG_HH

# include <QDir>
# include <QTimer>
# include <QStringList>
# include <QProcess>
# include <QtGui>
# include <QProgressDialog>
# include <ui_edit_seg.h>

# ifndef INCLUDE_MLN_FILES
#  define MLN_INCLUDE_ONLY
# endif

# include <mln/core/image/image2d.hh>
# include <mln/core/image/imorph/labeled_image.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/util/array.hh>
# include <mln/util/set.hh>

namespace mln
{

  namespace demo
  {

    class edit_seg : public QWidget, private Ui::EditSeg
    {
      Q_OBJECT

    public:
      edit_seg(QWidget *parent = 0);
      ~edit_seg();


    private slots:
      void on_browseBtn_clicked(bool);
      void on_saveBtn_clicked(bool);
      void compute_image(int sli);
      void join_components(const QPointF& p1, const QPointF& p2);
      void select_component(const QPointF& p);

    private: // Members


    private: // Attributes
      image3d<value::int_u8> seg_;
      value::int_u8 nlabels_;
      image3d<value::rgb8> seg_rgb8_;
      mln::util::array<box2d> bboxes_;

      mln::util::array<bool> fselected_;
      mln::util::set<value::int_u8> selected_;

      int current_sli_;
    };


  } // end of namespace mln::demo

} // end of namespace mln

#endif // ! MLN_DEMO_SRC_EDIT_SEG_HH
