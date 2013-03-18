// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WORLD_RGB_INVERT_SPE_HH
# define MLN_WORLD_RGB_INVERT_SPE_HH

/// \file
///
/// Specializations for mln::world::rgb::invert.

# ifndef MLN_WORLD_RGB_INVERT_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_WORLD_RGB_INVERT_HH

# include <mln/core/concept/image.hh>
# include <mln/trait/value_.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace world
  {

    namespace rgb
    {

      namespace impl
      {

	template <typename I, typename O>
	inline
	void invert_rgb_fastest(const Image<I>& input_, Image<O>& output_)
	{
	  mln_trace("world::rgb::impl::invert_rgb_fastest");

	  const I& input = exact(input_);
	  O& output = exact(output_);

	  mln_precondition(input.is_valid());
	  mln_precondition(output.is_valid());

	  typedef mln_value(I) V;
	  typedef typename V::red_t Red;
	  typedef typename V::green_t Green;
	  typedef typename V::blue_t Blue;

	  mln_pixter(const I) ip(input);
	  mln_pixter(O)       op(output);
	  for_all_2(ip, op)
	  {
	    op.val().red() = mln_min(Red) + (mln_max(Red) - ip.val().red());
	    op.val().green() = mln_min(Green) + (mln_max(Green) - ip.val().green());
	    op.val().blue() = mln_min(Blue) + (mln_max(Blue) - ip.val().blue());
	  }

	  mln_trace("world::rgb::impl::invert_rgb_fastest");
	}


	template <typename I, typename O>
	inline
	void invert_rgb(const Image<I>& input_, Image<O>& output_)
	{
	  mln_trace("world::rgb::impl::invert_rgb");

	  const I& input = exact(input_);
	  O& output = exact(output_);

	  mln_precondition(input.is_valid());
	  mln_precondition(output.is_valid());

	  typedef mln_value(I) V;
	  typedef typename V::red_t Red;
	  typedef typename V::green_t Green;
	  typedef typename V::blue_t Blue;

	  mln_piter(I) p(input.domain());
	  for_all(p)
	  {
	    output(p).red() = mln_min(Red) + (mln_max(Red) - input(p).red());
	    output(p).green() = mln_min(Green) + (mln_max(Green) - input(p).green());
	    output(p).blue() = mln_min(Blue) + (mln_max(Blue) - input(p).blue());
	  }

	}


      } // end of namespace mln::world::rgb::impl

    } // end of namespace mln::world::rgb

  } // end of namespace mln::world

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_WORLD_RGB_INVERT_SPE_HH
