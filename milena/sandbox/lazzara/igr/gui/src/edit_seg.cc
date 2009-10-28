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


#include <QtGui>

# define INCLUDE_MLN_FILES
#include <src/edit_seg.hh>
#include <src/to_qimage.hh>

#include <mln/core/image/image3d.hh>
#include <mln/core/image/imorph/labeled_image.hh>
#include <mln/core/concept/function.hh>

#include <mln/data/wrap.hh>
#include <mln/data/transform_inplace.hh>

#include <mln/value/int_u16.hh>
#include <mln/value/int_u8.hh>

#include <mln/io/ppm/save.hh>
#include <mln/io/dump/load.hh>

#include <mln/literal/colors.hh>

#include <mln/labeling/colorize.hh>

#include <mln/fun/v2v/wrap.hh>

namespace mln
{

//   namespace internal
//   {

//     struct merge_component : Function_v2v<merge_component>
//     {
//       typedef value::int_u16 result;

//       merge_component(const value::int_u16& replaced,
// 		      const value::int_u16& by)
// 	: replaced_(replaced), by_(by)
//       {
//       }

//       value::int_u16 operator()(const value::int_u16& v) const
//       {
// 	if (v == replaced_)
// 	  return by_;
// 	return v;
//       }

//       value::int_u16 replaced_;
//       value::int_u16 by_;
//     };

//   } // end of namespace mln::internal




  namespace demo
  {


    edit_seg::edit_seg(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);

      connect(viewer, SIGNAL(slider_valueChanged(int)),
	      this, SLOT(compute_image()));

      connect(viewer, SIGNAL(mouse_draw_line(const QPointF&, const QPointF&)),
	      this, SLOT(join_components(const QPointF&, const QPointF&)));

      connect(viewer, SIGNAL(mouse_click(const QPointF&)),
	      this, SLOT(select_component(const QPointF&)));
    }

    edit_seg::~edit_seg()
    {

    }


    void edit_seg::on_browseBtn_clicked(bool)
    {
      QString
	filename = QFileDialog::getOpenFileName(this,
	  tr("Open Image."),
	  QString(),
	  tr("Images (*.dump)"));

      if (!filename.isEmpty())
	filepath->setText(filename);

      // Initial load.
      image2d<value::int_u16> tmp;
      io::dump::load(tmp, filepath->text().toStdString());
      seg_ = labeled_image<image2d<value::int_u16> >(tmp);

      selected_.resize(seg_.nlabels(), false);

      image2d<value::int_u8> tmp2 = data::wrap(value::int_u8(), seg_);
      seg_rgb8_ = data::convert(value::rgb8(), tmp2);

      viewer->set_image_layer_count(1);
    }


    void edit_seg::compute_image()
    {
      QImage ima = to_qimage(seg_rgb8_);
      viewer->update_image(ima);
    }


    void edit_seg::join_components(const QPointF& p1, const QPointF& p2)
    {
//       point2d
// 	mln_p1 = point2d(p1.y(), p1.x()),
// 	mln_p2 = point2d(p2.y(), p2.x());
//       if (seg_.domain().has(mln_p1) && seg_.domain().has(mln_p2) && // in domain
// 	  seg_(mln_p1) != seg_(mln_p2) // different components
// 	  && seg_(mln_p1) != 0 && seg_(mln_p2) != 0) // Not the wsl
//       {
// 	std::cout << "Merging " << seg_(mln_p1)
// 		  << " and " << seg_(mln_p2) << std::endl;
// 	std::cout << "mln_p1 = " << mln_p1 << " - mln_p2 = " << mln_p2 << std::endl;

// 	internal::merge_component f(seg_(mln_p1), seg_(mln_p2));
// 	data::transform_inplace(seg_, f);
// 	compute_image();
//       }
    }

    void edit_seg::on_saveBtn_clicked(bool)
    {
      QString
	filename = QFileDialog::getOpenFileName(this,
						tr("Save Image."),
						QString(),
						tr("Images (*.dump)"));


      image2d<value::int_u16>
	out = data::transform(seg_.unmorph_(), selected_);

      io::dump::save(out, filename.toStdString());
    }


    void edit_seg::select_component(const QPointF& p)
    {
      point2d
	mln_p = point2d(p.y(), p.x());

      if (seg_.domain().has(mln_p)
	  && !selected_(seg_(mln_p))
	  && seg_(mln_p) != 0) // Not the wsl
      {
	selected_(seg_(mln_p)) = true;
	data::fill((seg_rgb8_ | seg_.subdomain(seg_(mln_p))).rw(),
		   literal::red);
      }
      else
      {
	selected_(seg_(mln_p)) = false;
	value::rgb8
	  v = convert::to<value::rgb8>(fun::v2v:wrap<value::int_u8>(seg_(mln_p)));

	value::rgb8 v(seg_(mln_p), seg_(mln_p), seg_(mln_p));
	data::fill((seg_rgb8_ | seg_.subdomain(seg_(mln_p))).rw(), v);
      }
      compute_image();
    }


  } // end of namespace mln::demo

} // end of namespace mln
