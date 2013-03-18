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

#ifndef MLN_WORLD_INTER_PIXEL_COMPUTE_HH
# define MLN_WORLD_INTER_PIXEL_COMPUTE_HH

/// \file
///
/// FIXME: insert comment.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/trait/ch_value.hh>

# include <mln/world/inter_pixel/is_separator.hh>
# include <mln/world/inter_pixel/separator_to_pixels.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {


      template <typename I, typename F>
      image_if<mln_ch_value(mln_unmorph(I), mln_result(F)), is_separator>
      compute(const Image<I>& input, const Function_vv2v<F>& f);



# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename F>
      inline
      image_if<mln_ch_value(mln_unmorph(I), mln_result(F)), is_separator>
      compute(const Image<I>& input_, const Function_vv2v<F>& f_)
      {
	mln_trace("world::inter_pixel::compute");
	
	const I& input = exact(input_);
	const F& f     = exact(f_);

	mln_precondition(input.is_valid());

	typedef mln_unmorph(I) I_;
	typedef mln_ch_value(I_, mln_result(F)) O_;
	O_ output_;
	initialize(output_, input.unmorph_());

	typedef image_if<O_, is_separator> O;
	O output(output_, is_separator());
	
	mln_piter(O) e(output.domain());
	for_all(e)
	{
	  mln_site(I) p1, p2;
 	  separator_to_pixels(e, p1, p2);
	  output(e) = f(input(p1), input(p2));
	}

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_COMPUTE_HH
