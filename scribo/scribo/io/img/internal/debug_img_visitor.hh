// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_IO_IMG_INTERNAL_DEBUG_IMG_VISITOR_HH
# define SCRIBO_IO_IMG_INTERNAL_DEBUG_IMG_VISITOR_HH

/// \file
///
/// Save document image analysis to a small image.

# include <fstream>

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/draw/box.hh>
# include <mln/subsampling/antialiased.hh>
# include <mln/morpho/elementary/gradient_external.hh>

# include <scribo/core/internal/doc_serializer.hh>
# include <scribo/core/document.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/line_info.hh>

# include <scribo/io/img/internal/draw_edges.hh>

namespace scribo
{

  namespace io
  {

    namespace img
    {

      namespace internal
      {


	class debug_img_visitor : public doc_serializer<debug_img_visitor>
	{
	public:
	  // Constructor
	  debug_img_visitor(mln::image2d<value::rgb8>& out,
			    unsigned output_ratio);

	  // Visit overloads
	  template <typename L>
	  void visit(const document<L>& doc) const;

	  void visit(const component_info& info) const;

	  template <typename L>
	  void visit(const paragraph_set<L>& parset) const;

	  template <typename L>
	  void visit(const line_info<L>& line) const;

	private: // Attributes
	  mln::image2d<value::rgb8>& output;
	  unsigned output_ratio;

	  mutable image2d<scribo::def::lbl_type> elt_edge;


	private: // Methods
	  box2d compute_bbox(const box2d& b) const;
	};



# ifndef MLN_INCLUDE_ONLY


	inline
	box2d
	debug_img_visitor::compute_bbox(const box2d& b) const
	{
	  point2d
	    pmin = b.pmin() / output_ratio,
	    pmax = b.pmax() / output_ratio;

	  return box2d(pmin, pmax);
	}


	inline
	debug_img_visitor::debug_img_visitor(mln::image2d<value::rgb8>& out,
					     unsigned output_ratio)
	  : output(out), output_ratio(output_ratio)
	{
	  mln_assertion(output.is_valid());
	}


	/// Document
	//
	template <typename L>
	void
	debug_img_visitor::visit(const document<L>& doc) const
	{
	  // Text
	  if (doc.has_text())
	    doc.paragraphs().accept(*this);

	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    // Prepare element edges

	    // FIXME: UGLY! Too slow!
	    scribo::def::lbl_type nlabels;
	    component_set<L> elts = primitive::extract::components(
	      data::convert(bool(), mln::subsampling::antialiased(doc.elements().labeled_image(),
								  output_ratio)),
	      c8(),
	      nlabels);

	    // Preserving elements tags
	    if (doc.elements().nelements() != elts.nelements())
	    {
	      std::cerr << "Warnig: could not preserve element type in "
			<< "img debug output." << std::endl;
	      std::cerr << "The number of non text element has changed while "
			<< "subsampling images : "
			<< doc.elements().nelements() << " vs "
			<< elts.nelements() << std::endl;
	    }
	    else
	      for_all_comps(c, doc.elements())
		elts(c).update_type(doc.elements()(c).type());

	    elt_edge = morpho::elementary::gradient_external(elts.labeled_image(), c8());

//	    const component_set<L>& elts = doc.elements();
	    for_all_comps(e, elts)
	      if (elts(e).is_valid())
		elts(e).accept(*this);
	  }


	  // line seraparators
	  if (doc.has_vline_seps())
	    for_all_comps(c, doc.vline_seps_comps())
	      doc.vline_seps_comps()(c).accept(*this);
	  if (doc.has_hline_seps())
	    for_all_comps(c, doc.hline_seps_comps())
	      doc.hline_seps_comps()(c).accept(*this);

	}


	/// Component_info
	//
	inline
	void
	debug_img_visitor::visit(const component_info& info) const
	{
	  switch (info.type())
	  {
	    case component::HorizontalLineSeparator:
	    case component::VerticalLineSeparator:
	    {
	      mln::draw::box(output, compute_bbox(info.bbox()),
			     literal::cyan);
	    }
	    break;


	    default:
	    case component::Image:
	    {
	      // The bbox does not need to be reajusted to the
	      // subsampled domain since it has been recomputed while
	      // computing the edge image.
	      //
	      // However, the bbox must be enlarged since only the
	      // _external_ edge is computed.
	      box2d b = info.bbox();
	      b.enlarge(1);
	      b.crop_wrt(output.domain());
	      data::fill(((output | b).rw()
			  | (pw::value(elt_edge)
			     == pw::cst((scribo::def::lbl_type)info.id().to_equiv()))).rw(),
			 literal::orange);
	    }
	    break;
	  }
	}

	/// Paragraph Set
	//
	template <typename L>
	void
	debug_img_visitor::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  for_all_paragraphs(p, parset)
	  {
	    const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	    for_all_paragraph_lines(lid, line_ids)
	    {
	      line_id_t l = line_ids(lid);
	      lines(l).accept(*this);
	    }

	    box2d b = compute_bbox(parset(p).bbox());
	    b.enlarge(1);
	    b.crop_wrt(output.domain());
	    mln::draw::box(output, b, literal::blue);
	  }
	}


	template <typename L>
	void
	debug_img_visitor::visit(const line_info<L>& line) const
	{
	  point2d
	    pmin = line.bbox().pmin(),
	    pmax = line.bbox().pmax();
	  pmax.row() = line.baseline();
	  pmin.row() = line.baseline();

	  pmin = pmin / output_ratio;
	  pmax = pmax / output_ratio;

	  mln::draw::line(output, pmin, pmax, literal::red);
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::img::internal

    } // end of namespace scribo::io::img

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_IMG_INTERNAL_DEBUG_IMG_VISITOR_HH
