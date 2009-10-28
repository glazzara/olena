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
#include <src/display.hh>
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

    display::display(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);

      connect(viewer, SIGNAL(slider_valueChanged(int)),
	      this, SLOT(compute_image(int)));
    }

    display::~display()
    {
    }


    // Private slots

    void display::on_browseBtn_clicked(bool)
    {
      QString
	filename = QFileDialog::getOpenFileName(this,
	  tr("Open Image."),
	  QString(),
	  tr("Images (*.dcm)"));

      if (!filename.isEmpty())
	filepath->setText(filename);
    }

    void display::compute_image(int sli)
    {
      slice_image<dcm_ima_t> sl_ima = slice(dcm_ima, sli);

      QImage ima = to_qimage(data::convert(value::rgb8(), sl_ima));

      viewer->update_image(ima);
    }


    void display::on_loadBtn_clicked(bool)
    {
      load_dicom(filepath->text());

      viewer->set_image_layer_count(geom::nslis(dcm_ima));
    }



    // Private members

    void display::load_dicom(const QString& filename)
    {
      image3d<value::int_u12> tmp;
      io::dicom::load(tmp, filename.toStdString());
      dcm_ima = to_int_u8(tmp);
    }


    template <typename I>
    mln_ch_value(I, value::int_u8) display::to_int_u8(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      mln_ch_value(I, value::int_u8)
	tmp = data::stretch(value::int_u8(), ima);

      return tmp;
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
