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

#ifndef MLN_DEMO_SRC_DISPLAY_SEG_HH

# include <QDir>
# include <QTimer>
# include <QStringList>
# include <QProcess>
# include <QtGui>
# include <QProgressDialog>
# include <ui_display_seg.h>

# ifndef INCLUDE_MLN_FILES
#  define MLN_INCLUDE_ONLY
# endif

# include <mln/core/image/image3d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/int_u16.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/hsl.hh>

namespace mln
{

  namespace demo
  {

    class display_seg : public QWidget, private Ui::DisplaySeg
    {
      Q_OBJECT

      typedef image3d<value::int_u8> dcm_ima_t;
      typedef image2d<value::int_u16> seg_ima_t;
      typedef image2d<value::hsl_f> result_t;

    public:
      display_seg(QWidget *parent = 0);
      ~display_seg();


    private slots:
      void on_browseBtn_clicked(bool);
      void on_browseSegBtn_clicked(bool);
      void compute_image(int sli);
      void on_loadBtn_clicked(bool);

    private: // Members

      void setup_result();
      void load_seg(const QString& filename);
      void load_dicom(const QString& filename);

      template <typename I>
      mln_ch_value(I, value::int_u8) to_int_u8(const Image<I>& ima_);

      void prepare_and_send_result(const QString& output);

    private: // Attributes
      dcm_ima_t dcm_ima;
      seg_ima_t seg_ima;
      value::int_u8 nlabels_;
      result_t result;

    };


  } // end of namespace mln::demo

} // end of namespace mln

#endif // ! MLN_DEMO_SRC_DISPLAY_SEG_HH
