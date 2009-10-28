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
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/concept/function.hh>

#include <mln/geom/nslis.hh>

#include <mln/data/wrap.hh>
#include <mln/data/transform_inplace.hh>

#include <mln/value/int_u8.hh>

#include <mln/io/ppm/save.hh>
#include <mln/io/dump/load.hh>

#include <mln/literal/colors.hh>

#include <mln/labeling/colorize.hh>
#include <mln/labeling/relabel.hh>

#include <mln/fun/v2v/wrap.hh>

#include <mln/accu/stat/max.hh>

namespace mln
{

//   namespace internal
//   {

//     struct merge_component : Function_v2v<merge_component>
//     {
//       typedef value::int_u8 result;

//       merge_component(const value::int_u8& replaced,
// 		      const value::int_u8& by)
// 	: replaced_(replaced), by_(by)
//       {
//       }

//       value::int_u8 operator()(const value::int_u8& v) const
//       {
// 	if (v == replaced_)
// 	  return by_;
// 	return v;
//       }

//       value::int_u8 replaced_;
//       value::int_u8 by_;
//     };

//   } // end of namespace mln::internal




  namespace demo
  {


    edit_seg::edit_seg(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);

      connect(viewer, SIGNAL(slider_valueChanged(int)),
	      this, SLOT(compute_image(int)));

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
	filename = QFileDialog::getOpenFileName(0,
	  tr("Open Image."),
	  QString(),
	  tr("Images (*.dump)"));

      if (!filename.isEmpty())
      {
	filepath->setText(filename);

	// Release memory
  	seg_.destroy();
  	seg_rgb8_.destroy();

	io::dump::load(seg_, filepath->text().toStdString());

	mln_assertion(seg_.is_valid());

	nlabels_ = data::compute(accu::meta::stat::max(), seg_);

	fselected_.resize(nlabels_, false);
	selected_.clear();

 	seg_rgb8_ = data::convert(value::rgb8(),
 				  data::wrap(value::int_u8(), seg_));

  	viewer->set_image_layer_count(geom::nslis(seg_));
      }
    }


    void edit_seg::compute_image(int sli)
    {
      current_sli_ = sli;

      slice_image<image3d<value::int_u8> > sl_ima = slice(seg_, sli);

      bboxes_ = labeling::compute(accu::meta::shape::bbox(), sl_ima, nlabels_);


      slice_image<image3d<value::rgb8> > sl_ima_rgb = slice(seg_rgb8_, sli);
      for (unsigned i = 0; i < selected_.nelements(); ++i)
	if (selected_[i] < bboxes_.nelements()
	    && bboxes_(selected_[i]).is_valid())
	  data::fill(((sl_ima_rgb | bboxes_(selected_[i])).rw()
		      | (pw::value(sl_ima) == pw::cst(selected_[i]))).rw(),
		     literal::red);

      QImage ima = to_qimage(duplicate(sl_ima_rgb));
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
	filename = QFileDialog::getSaveFileName(this,
						tr("Save Image."),
						QString(),
						tr("Images (*.dump)"));

      if (!filename.isEmpty())
      {
	value::int_u8 new_nlabels;
	image3d<value::int_u8>
	  out = labeling::relabel(seg_, nlabels_,
				  new_nlabels, fselected_);

	io::dump::save(out, filename.toStdString());
      }

    }


    void edit_seg::select_component(const QPointF& p)
    {
      point3d
	mln_p(current_sli_, p.y(), p.x());

      slice_image<image3d<value::rgb8> >
	sl_ima_rgb = slice(seg_rgb8_, current_sli_);
      slice_image<image3d<value::int_u8> >
	sl_ima = slice(seg_, current_sli_);

      if (seg_.domain().has(mln_p))
      {
	value::int_u8 v = seg_(mln_p);

	if (!fselected_(v) && v != 0) // Not the wsl
	{
	  fselected_(v) = true;
	  selected_.insert(v);
	  data::fill(((sl_ima_rgb | bboxes_(v)).rw()
		      | (pw::value(sl_ima) == pw::cst(v))).rw(),
		     literal::red);
	}
	else
	{
	  fselected_(v) = false;
	  selected_.remove(v);

//	  fun::v2v::wrap<value::int_u8> f;
//	  value::int_u8 v8 = f(v);
	  value::rgb8 v_rgb(v, v, v);
	  data::fill(((sl_ima_rgb | bboxes_(v)).rw()
		      | (pw::value(sl_ima) == pw::cst(v))).rw(), v_rgb);
	}
      }

      saveBtn->setEnabled(selected_.nelements() > 0);
      compute_image(current_sli_);
    }


  } // end of namespace mln::demo

} // end of namespace mln
