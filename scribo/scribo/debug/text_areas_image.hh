// Copyright (C) 2010, 2011, 2014 EPITA Research and Development Laboratory
// (LRDE).
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


#ifndef SCRIBO_DEBUG_TEXT_AREAS_IMAGE_HH
# define SCRIBO_DEBUG_TEXT_AREAS_IMAGE_HH

/// \file
///
/// \brief Compute an image including detected text areas only.

# include <mln/core/concept/image.hh>

# include <mln/core/image/imorph/tr_image.hh>

# include <mln/core/routine/duplicate.hh>

# include <mln/data/paste.hh>
# include <mln/data/fill.hh>

# include <mln/fun/x2x/translation.hh>

# include <mln/math/max.hh>

# include <mln/literal/black.hh>

# include <scribo/core/component_set.hh>

namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /*! \brief Compute an image including detected text areas only.

      \ingroup grpalgodebug
     */
    template <typename I, typename L>
    mln_concrete(I)
    text_areas_image(const Image<I>& input_rgb,
		     const scribo::component_set<L>& comps);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename L>
    mln_concrete(I)
    text_areas_image(const Image<I>& input_rgb_,
		     const scribo::component_set<L>& comps)
    {
      mln_trace("scribo::debug::text_areas_image");

      const I& input_rgb = exact(input_rgb_);
      mln_precondition(input_rgb.is_valid());

      typedef mln_site(I) P;

      unsigned shift = 5;
      float height = 1, width = 0;
      for_all_comps(i, comps)
	if (comps(i).is_valid())
	{
	  height += comps(i).bbox().nrows() + shift;
	  width = math::max(static_cast<float>(comps(i).bbox().ncols()),
			    width);
	}
      if (width == 0)
	width = 1;

      I output(height, width);
      data::fill(output, literal::black);

      algebra::vec<2, float> dv;
      dv[0] = 0;
      dv[1] = 0;
      for_all_comps(i, comps)
	if (comps(i).is_valid())
	{
          typedef mln::sub_image<I, box2d> tmp_t;
          tmp_t tmp = duplicate(input_rgb | comps(i).bbox());

	  typedef mln::fun::x2x::translation<P::dim, float> trans_t;
	  trans_t trans(dv - comps(i).bbox().pmin().to_vec());

	  mln_domain(I)
	    tr_box(comps(i).bbox().pmin().to_vec() + trans.t(),
		   comps(i).bbox().pmax().to_vec() + trans.t());

	  typedef mln_domain(I) domain_t;
	  tr_image<domain_t, tmp_t, trans_t> tr_ima(tr_box, tmp, trans);

	  data::paste(tr_ima, output);
	  dv[0] += comps(i).bbox().nrows() + shift;
	}

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_TEXT_AREAS_IMAGE_HH
