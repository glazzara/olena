// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file
///
/// \brief Find in a document non text which are not text.
///
/// \fixme To be optimized!

#ifndef SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_KMEAN_HH
# define SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_KMEAN_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/data/fill.hh>
# include <mln/util/array.hh>
# include <mln/labeling/compute.hh>
# include <mln/labeling/relabel.hh>
# include <mln/accu/math/count.hh>
# include <mln/pw/all.hh>

# include <mln/draw/box_plain.hh>

# include <mln/value/label_8.hh>
# include <mln/value/rgb.hh>
# include <mln/value/rgb8.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/document.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/def/lbl_type.hh>
# include <scribo/filter/objects_small.hh>

# include <mln/clustering/kmean_rgb.hh>
# include <mln/fun/v2v/rgb8_to_rgbn.hh>

namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;


      template <typename L, typename I>
      component_set<L>
      non_text_kmean(const document<L>& doc, const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	template <typename L>
	struct order_bbox
	{
	  order_bbox(const scribo::component_set<L>& comps)
	    : comps_(comps)
	  {
	  }

	  bool operator()(const unsigned& c1, const unsigned& c2) const
	  {
	    if (comps_(c1).bbox().nsites() == comps_(c2).bbox().nsites())
	      return c1 > c2;
	    return comps_(c1).bbox().nsites() > comps_(c2).bbox().nsites();
	  }

	  scribo::component_set<L> comps_;
	};

      } // end of namespace scribo::primitive::extract::internal



      // FACADE

      template <typename L, typename I>
      component_set<L>
      non_text_kmean(const document<L>& doc, const Image<I>& input_)
      {
	mln_trace("scribo::primitive::extract::non_text_kmean");

	const I& input = exact(input_);
	mln_precondition(doc.is_valid());
	mln_precondition(input.is_valid());

	const line_set<L>& lines = doc.lines();

	// Element extraction

	image2d<value::label_8> img_lbl8;
	{
	  image2d<bool> content;
	  initialize(content, input);
	  data::fill(content, true);

	  for_all_lines(l, lines)
	    if (lines(l).type() == line::Text)
	      data::fill((content | lines(l).bbox()).rw(), false);

	  typedef mln::value::rgb<5>                 t_rgb5;
	  typedef mln::fun::v2v::rgb8_to_rgbn<5>     t_rgb8_to_rgb5;

	  image2d<t_rgb5>
	    img_rgb5 = mln::data::transform(doc.image(), t_rgb8_to_rgb5());

	  img_lbl8 =
	    mln::clustering::kmean_rgb<double,5>(
	      (img_rgb5 | pw::value(content)), 3, 10, 10).unmorph_();
	  data::fill((img_lbl8 | !pw::value(content)).rw(), 0u);

	  mln::util::array<unsigned>
	    card = mln::labeling::compute(accu::math::count<value::label_8>(),
					  img_lbl8, img_lbl8, 3);

	  unsigned max = 0, bg_id = 0;
	  for_all_ncomponents(c, 3)
	    if (card(c) > max)
	    {
	      max = card(c);
	      bg_id = c;
	    }

	  mln::fun::i2v::array<bool> f(4, true);
	  f(0) = false;
	  f(bg_id) = false;
	  labeling::relabel_inplace(img_lbl8, 4, f);
	}


	component_set<L> output;

	std::cout << "Removing small elements" << std::endl;
	{
	  image2d<bool> elts;
	  initialize(elts, img_lbl8);
	  data::fill(elts, false);
	  data::fill((elts | (pw::value(img_lbl8) != pw::cst(0))).rw(), true);

	  scribo::def::lbl_type nlabels;
	  elts = filter::components_small(elts, c8(), nlabels, 40);

	  output = primitive::extract::components(elts, c8(), nlabels);
	}


	std::cout << "Ignoring inner elements" << std::endl;

	{
	  // FIXME: We would like to use the convex hull instead of the bbox.
	  internal::order_bbox<L> func(output);
	  util::array<unsigned> box_ordered_comps;
	  for (unsigned i = 1; i < output.nelements(); ++i)
	    box_ordered_comps.append(i);
	  std::sort(box_ordered_comps.hook_std_vector_().begin(),
		    box_ordered_comps.hook_std_vector_().end(), func);

	  image2d<bool> merged_elts;
	  initialize(merged_elts, img_lbl8);
	  data::fill(merged_elts, false);
	  for (unsigned i = 0; i < box_ordered_comps.nelements(); ++i)
	  {
	    unsigned c = box_ordered_comps(i);
	    point2d
	      pminright = output(c).bbox().pmin(),
	      pmaxleft = output(c).bbox().pmax();
	    pminright.col() = output(c).bbox().pmax().col();
	    pmaxleft.col() = output(c).bbox().pmin().col();

	    if (merged_elts(output(c).bbox().pmin())
		&& merged_elts(output(c).bbox().pmax())
		&& merged_elts(pminright)
		&& merged_elts(pmaxleft))
	      output(c).update_tag(component::Ignored);
	    else
	      mln::draw::box_plain(merged_elts, output(c).bbox(), true);
	  }
	}

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_KMEAN_HH
