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

#include <QApplication>
#include <QtCore>
#include <QtGui>

# define INCLUDE_MLN_FILES
#include <src/display_seg.hh>
#include <src/image_viewer.hh>

#include <src/to_qimage.hh>

#include <mln/accu/count_value.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/image/image3d.hh>
#include <mln/border/resize.hh>
#include <mln/data/compute.hh>
#include <mln/logical/not.hh>
#include <mln/value/hsl.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u32.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/io/dicom/all.hh>
#include <mln/io/dump/all.hh>


#include <mln/io/ppm/save.hh>



namespace mln
{

  namespace demo
  {

    display_seg::display_seg(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);

      connect(viewer, SIGNAL(slider_valueChanged(int)),
	      this, SLOT(compute_image(int)));
    }

    display_seg::~display_seg()
    {
    }


    // Private slots

    void display_seg::on_browseBtn_clicked(bool)
    {
      QString
	filename = QFileDialog::getOpenFileName(this,
	  tr("Open Image."),
	  QString(),
	  tr("Images (*.dcm)"));

      if (!filename.isEmpty())
	filepath->setText(filename);
    }


    void display_seg::on_browseSegBtn_clicked(bool)
    {
      QString
	filename = QFileDialog::getOpenFileName(this,
	  tr("Open Image."),
	  QString(),
	  tr("Images (*.dump)"));

      if (!filename.isEmpty())
	segfilepath->setText(filename);
    }


    void display_seg::compute_image(int sli)
    {
      slice_image<dcm_ima_t> sl_ima = slice(dcm_ima, sli);

      mln_piter_(result_t) p(result.domain());
      for_all(p)
	result(p).lum() = (sl_ima(p) / static_cast<float>(mln_max(value::int_u8))) / 2.0;

      QImage ima = to_qimage(data::convert(value::rgb8(), result));

      viewer->update_image(ima);
    }


    void display_seg::on_loadBtn_clicked(bool)
    {
      load_dicom(filepath->text());
      load_seg(segfilepath->text());
      setup_result();

      viewer->set_image_layer_count(geom::nslis(dcm_ima));
    }



    // Private members

    void display_seg::setup_result()
    {
      initialize(result, seg_ima);
      mln_piter_(result_t) p(result.domain());
      for_all(p)
      {
	result(p).hue() = ((seg_ima(p) * 10) % 359) + 1;
	result(p).sat() = 1;
      }
    }

    void display_seg::load_seg(const QString& filename)
    {
      io::dump::load(seg_ima, filename.toStdString());
    }


    void display_seg::load_dicom(const QString& filename)
    {
      image3d<value::int_u12> tmp;
      io::dicom::load(tmp, filename.toStdString());
      dcm_ima = to_int_u8(tmp);
    }


    template <typename I>
    mln_ch_value(I, value::int_u8) display_seg::to_int_u8(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      mln_ch_value(I, value::int_u8)
	tmp = data::stretch(value::int_u8(), ima);

      return tmp;
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
