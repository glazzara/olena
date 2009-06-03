// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_WORLD_INTER_PIXEL_COMPUTE_HH
# define MLN_WORLD_INTER_PIXEL_COMPUTE_HH

/// \file mln/world/inter_pixel/compute.hh
///
/// FIXME: insert comment.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
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
	trace::entering("world::inter_pixel::compute");
	
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

	trace::exiting("world::inter_pixel::compute");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_COMPUTE_HH
