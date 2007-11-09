// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_DISPLAY_COLOR_PRETTY_HH
# define MLN_DISPLAY_COLOR_PRETTY_HH

/*! \file mln/display/color_pretty.hh
 *
 * \brief Create a pretty image, which display its content
 *  with red value for undefined point site.
 */

# include <mln/trait/image_from_mesh.hh>
# include <mln/core/image_if_value.hh>
# include <mln/core/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/level/fill.hh>
# include <mln/level/paste.hh>
# include <mln/core/p_set.hh>

namespace mln
{

  namespace display
  {

    template <typename I>
    typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
    color_pretty(const Image<I>& input_);

    template <typename I>
    typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
    color_pretty_rgb(const Image<I>& input_,
		     const p_set<mln_point(I) >& s1_,
		     const p_set<mln_point(I) >& s2_,
		     const p_set<mln_point(I) >& s3_);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename V>
      value::rgb8
      color_value(V v)
      {
	//r = v * (mln_max(V) / mln_max(value::int_u8) );
	return value::rgb8(v, v, v);
      }

      template <unsigned int n>
      value::rgb8
      color_value(value::rgb<n> v)
      {
	return v;
      }

      value::rgb8
      color_value(bool b)
      {
	value::int_u8 r = b ? 255 : 0;
	return value::rgb8(r, r, r);
      }


      template <typename I>
      typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
      color_pretty(const Image<I>& input_)
      {
	const I& input = exact (input_);

	image2d<value::rgb8> output(input.domain().bbox());
	level::fill(output, value::rgb8(255, 0, 0));

	{
	  mln_piter(I) p(input.domain());

	  for_all(p)
	    output(p) = value::rgb8(color_value(input(p)));
	}
	return output;
      }

      template <typename I>
      typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
      color_pretty_rgb(const Image<I>& input_,
		       const p_set<mln_point(I) >& s1_,
		       const p_set<mln_point(I) >& s2_,
		       const p_set<mln_point(I) >& s3_)
      {
	const I& input = exact (input_);

	image2d<value::rgb8> output(input.domain().bbox());
	level::fill(output, value::rgb8(0, 0, 0));

	{
	  mln_piter(p_set<mln_point(I) >) p(s1_);

	  for_all(p)
	    output(p).red() = 255;
	}

	{
	  mln_piter(p_set<mln_point(I) >) p(s2_);

	  for_all(p)
	    output(p).green() = 255;
	}

	{
	  mln_piter(p_set<mln_point(I) >) p(s3_);

	  for_all(p)
	    output(p).blue() = 255;
	}
	return output;
      }


    } // end of namespace mln::display::impl

    /// Facade.
    template <typename I>
    typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
    color_pretty(const Image<I>& input_)
    {
      return impl::color_pretty(input_);
    }

    template <typename I>
    typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
    color_pretty_rgb(const Image<I>& input_,
		     const p_set<mln_point(I) >& s1_,
		     const p_set<mln_point(I) >& s2_,
		     const p_set<mln_point(I) >& s3_)
    {
      return impl::color_pretty_rgb(input_, s1_, s2_, s3_);
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_COLOR_PRETTY_HH
